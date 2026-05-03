#include "orbitalcameracontrollercomponent.h"

#include <algorithm>

#include <yaml-cpp/yaml.h>

#include "components/cameracomponent.h"
#include "core/node.h"
#include "transformations/rotation.h"
#include "transformations/translation.h"

OrbitalCameraControllerComponent::OrbitalCameraControllerComponent(InputHandler *inputHandler,
                                                                   float maxPitch, float maxYaw,
                                                                   float lookSensitivity,
                                                                   Vec3 orbitTarget,
                                                                   float orbitRadius)
    : inputHandler_(inputHandler), maxPitch_(maxPitch), maxYaw_(maxYaw),
      lookSensitivity_(lookSensitivity), pitch_(0.0f), yaw_(0.0f), orbitTarget_(orbitTarget),
      orbitRadius_(orbitRadius) {}

void OrbitalCameraControllerComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
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
    if (data["orbitRadius"])
        orbitRadius_ = data["orbitRadius"].as<float>();
    if (data["orbitTarget"] && data["orbitTarget"].IsSequence())
        orbitTarget_ = Vec3(data["orbitTarget"][0].as<float>(), data["orbitTarget"][1].as<float>(),
                            data["orbitTarget"][2].as<float>());
}

void OrbitalCameraControllerComponent::onUpdate(float dt) {
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
    Transformation finalTransformation;
    Translation radiusTranslation(Vec3(0.0f, 0.0f, orbitRadius_));
    Translation targetTranslation(orbitTarget_);

    finalTransformation = targetTranslation * finalCameraRotation * radiusTranslation;

    node->setTransform(finalTransformation);
}