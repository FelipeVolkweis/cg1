#ifndef CAMERACONTROLLERCOMPONENT_H
#define CAMERACONTROLLERCOMPONENT_H

#include <memory>

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "types/vec3.h"
#include "window/inputhandler.h"

class CameraControllerComponent : public BaseComponent {
public:
    CameraControllerComponent() = default;
    CameraControllerComponent(InputHandler *inputHandler, float maxPitch, float maxYaw,
                              float lookSensitivity);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    void onUpdate(float dt) override;

private:
    InputHandler *inputHandler_ = nullptr;
    float maxPitch_ = 89.0f;
    float maxYaw_ = 360.0f;
    float lookSensitivity_ = 50.0f;

    float pitch_ = 0.0f;
    float yaw_ = 180.0f;
};

#endif
