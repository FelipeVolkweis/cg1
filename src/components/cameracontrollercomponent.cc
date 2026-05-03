#include "cameracontrollercomponent.h"

#include <algorithm>

#include <yaml-cpp/yaml.h>

#include "components/cameracomponent.h"
#include "core/node.h"
#include "transformations/rotation.h"
#include "transformations/translation.h"

CameraControllerComponent::CameraControllerComponent(InputHandler *inputHandler, float maxPitch,
                                                     float maxYaw, float lookSensitivity)
    : inputHandler_(inputHandler), maxPitch_(maxPitch), maxYaw_(maxYaw),
      lookSensitivity_(lookSensitivity), pitch_(0.0f), yaw_(0.0f) {}

void CameraControllerComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                                     InputHandler &inputHandler) {
    inputHandler_ = &inputHandler;
    if (data["maxPitch"])
        maxPitch_ = data["maxPitch"].as<float>();
    if (data["maxYaw"])
        maxYaw_ = data["maxYaw"].as<float>();
    if (data["sensitivity"])
        lookSensitivity_ = data["sensitivity"].as<float>();
    if (data["yaw"])
        yaw_ = data["yaw"].as<float>();
    if (data["pitch"])
        pitch_ = data["pitch"].as<float>();
}

void CameraControllerComponent::onUpdate(float dt) {
    if (!isEnabled())
        return;

    auto node = getNode();
    if (!node || !inputHandler_)
        return;

    auto yawVec = inputHandler_->getSecondaryXAxisInputVector();
    auto pitchVec = inputHandler_->getSecondaryYAxisInputVector();

    float yawInput = yawVec.x() - yawVec.y();
    float pitchInput = pitchVec.x() - pitchVec.y();

    yaw_ += yawInput * lookSensitivity_ * dt;
    pitch_ += pitchInput * lookSensitivity_ * dt;

    pitch_ = std::clamp(pitch_, -maxPitch_, maxPitch_);
    yaw_ = std::clamp(yaw_, -maxYaw_, maxYaw_);

    RotationX rotX(pitch_);
    RotationY rotY(yaw_);

    Transformation finalCameraRotation = rotY * rotX;
    Vec3 pos = node->getTransformation().getPosition();
    Translation translation(pos);

    Transformation finalTransformation = translation * finalCameraRotation;
    node->setTransform(finalTransformation);
}