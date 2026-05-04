#ifndef CAMERACONTROLLERCOMPONENT_H
#define CAMERACONTROLLERCOMPONENT_H

#include <memory>

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "core/enableable.h"
#include "types/vec3.h"
#include "window/inputhandler.h"

/**
 * @class CameraControllerComponent
 * @brief Component that handles camera movement and rotation based on input.
 */
class CameraControllerComponent : public BaseComponent, public Enableable {
public:
    CameraControllerComponent() = default;

    /**
     * @brief Constructor for CameraControllerComponent.
     * @param inputHandler Pointer to the input handler.
     * @param maxPitch Maximum pitch angle.
     * @param maxYaw Maximum yaw angle.
     * @param lookSensitivity Look sensitivity.
     */
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
