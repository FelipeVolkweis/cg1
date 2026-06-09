#include "cameracomponent.h"

#include <cmath>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/node.h"
#include "window/inputhandler.h"
#include "window/window.h"

CameraComponent::CameraComponent(Perspective perspective, Vec3 focalPoint, Vec3 up)
    : focalPoint_(focalPoint), up_(up) {
    LookAt lookAt = {Vec3(0, 0, 0), focalPoint, up};
    camera_ = std::make_shared<Camera>(perspective, lookAt);
}

void CameraComponent::load(const YAML::Node &data) {
    Perspective p = {45.0f, 16.0f / 9.0f, 0.01f, 1000.0f};

    smoothTime_ = 0.25f;
    maxSpeed_ = std::numeric_limits<float>::infinity();
    rotationLambda_ = 3.0f;
    currentVelocity_ = Vec3(0, 0, 0);

    if (data["fov"])
        p.fovy = data["fov"].as<float>();
    if (data["aspect"])
        p.aspect = data["aspect"].as<float>();
    if (data["near"])
        p.zNear = data["near"].as<float>();
    if (data["far"])
        p.zFar = data["far"].as<float>();

    if (data["smoothTime"])
        smoothTime_ = data["smoothTime"].as<float>();
    if (data["maxSpeed"])
        maxSpeed_ = data["maxSpeed"].as<float>();
    if (data["rotationLambda"])
        rotationLambda_ = data["rotationLambda"].as<float>();

    focalPoint_ = Vec3(0, 0, 0);
    if (data["focal"] && data["focal"].IsSequence()) {
        focalPoint_ = Vec3(data["focal"][0].as<float>(), data["focal"][1].as<float>(),
                           data["focal"][2].as<float>());
    }

    up_ = Vec3(0, 1, 0);
    if (data["up"] && data["up"].IsSequence()) {
        up_ = Vec3(data["up"][0].as<float>(), data["up"][1].as<float>(), data["up"][2].as<float>());
    }

    LookAt lookAt = {Vec3(0, 0, 0), focalPoint_, up_};
    camera_ = std::make_shared<Camera>(p, lookAt);

    currentPosition_ = camera_->getPosition();
}

void CameraComponent::onUpdate(float dt) {
    auto node = getNode();
    if (node && camera_) {
        Transformation globalTransform;
        auto current = node;
        while (current != nullptr) {
            globalTransform = current->getTransformation() * globalTransform;
            current = current->getParent();
        }

        Mat4x4 mat = globalTransform.getTransformationMatrix();

        Vec3 targetPosition = mat.block<3, 1>(0, 3);

        currentPosition_ = smoothDamp(currentPosition_, targetPosition, currentVelocity_,
                                      smoothTime_, maxSpeed_, dt);

        Quaternion targetRotation(mat.block<3, 3>(0, 0));
        float blend = 1.0f - std::exp(-rotationLambda_ * dt);

        currentRotation_ = currentRotation_.slerp(blend, targetRotation);
        currentRotation_.normalize();

        Vec3 forward = currentRotation_ * Vec3(0.0f, 0.0f, -1.0f);
        Vec3 up = currentRotation_ * Vec3(0.0f, 1.0f, 0.0f);

        camera_->setPosition(currentPosition_);
        camera_->setFocalPoint(currentPosition_ + forward);
        camera_->setUp(up);

        auto dims = Window::instance().getInputHandler().getWindowDimensions();
        float aspect = static_cast<float>(dims.first) / static_cast<float>(dims.second);
        camera_->setAspect(aspect);
    }
}

Vec3 CameraComponent::smoothDamp(Vec3 currentPosition, Vec3 targetPosition, Vec3 &currentVelocity,
                                 float smoothTime, float maxSpeed, float deltaTime) {
    smoothTime = std::max(0.0001f, smoothTime);

    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

    Vec3 change = currentPosition - targetPosition;
    Vec3 originalTo = targetPosition;

    float maxChange = maxSpeed * smoothTime;
    float sqDist = change.squaredNorm();
    if (sqDist > maxChange * maxChange) {
        change = change.normalized() * maxChange;
    }

    targetPosition = currentPosition - change;

    Vec3 temp = (currentVelocity + change * omega) * deltaTime;
    currentVelocity = (currentVelocity - temp * omega) * exp;
    Vec3 output = targetPosition + (change + temp) * exp;

    if ((originalTo - currentPosition).dot(output - originalTo) > 0.0f) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}
