#include "cameracomponent.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/node.h"

CameraComponent::CameraComponent(Perspective perspective, Vec3 focalPoint, Vec3 up)
    : focalPoint_(focalPoint), up_(up) {
    LookAt lookAt = {Vec3(0, 0, 0), focalPoint, up};
    camera_ = std::make_shared<Camera>(perspective, lookAt);
}

void CameraComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                           InputHandler &inputHandler) {

    Perspective p = {45.0f, 16.0f / 9.0f, 0.1f, 100.0f};
    if (data["fov"])
        p.fovy = data["fov"].as<float>();
    if (data["aspect"])
        p.aspect = data["aspect"].as<float>();
    if (data["near"])
        p.zNear = data["near"].as<float>();
    if (data["far"])
        p.zFar = data["far"].as<float>();

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
        Vec3 position = mat.block<3, 1>(0, 3);
        Vec3 forward = mat.block<3, 1>(0, 2) * -1.0f;
        Vec3 up = mat.block<3, 1>(0, 1);

        camera_->setPosition(position);
        camera_->setFocalPoint(position + forward);
        camera_->setUp(up);
    }
}
