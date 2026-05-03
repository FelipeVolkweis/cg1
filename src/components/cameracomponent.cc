#include "cameracomponent.h"

#include <iostream>

#include "core/node.h"

CameraComponent::CameraComponent(Perspective perspective, Vec3 focalPoint, Vec3 up)
    : focalPoint_(focalPoint), up_(up) {
    LookAt lookAt = {Vec3(0, 0, 0), focalPoint, up};
    camera_ = std::make_shared<Camera>(perspective, lookAt);
}

std::shared_ptr<CameraComponent> CameraComponent::parse(std::istream &iss, Vec3 &outPosition) {
    std::string key;
    Perspective p = {45.0f, 16.0f / 9.0f, 0.1f, 100.0f};
    Vec3 focal(0, 0, 0);
    Vec3 up(0, 1, 0);
    outPosition = Vec3(0, 0, 0);

    while (iss >> key) {
        if (key == "perspective") {
            iss >> p.fovy >> p.aspect >> p.zNear >> p.zFar;
        } else if (key == "pos") {
            iss >> outPosition.x() >> outPosition.y() >> outPosition.z();
        } else if (key == "focal") {
            iss >> focal.x() >> focal.y() >> focal.z();
        } else if (key == "up") {
            iss >> up.x() >> up.y() >> up.z();
        }
    }

    return std::make_shared<CameraComponent>(p, focal, up);
}

void CameraComponent::onUpdate(float dt) {
    auto node = getNode();
    if (node) {
        Mat4x4 transform = node->getTransformation().getTransformationMatrix();
        Vec3 position = transform.block<3, 1>(0, 3);

        Vec3 forward = transform.block<3, 1>(0, 2) * -1.0f;
        Vec3 up = transform.block<3, 1>(0, 1);

        camera_->setPosition(position);
        camera_->setFocalPoint(position + forward);
        camera_->setUp(up);
    }
}
