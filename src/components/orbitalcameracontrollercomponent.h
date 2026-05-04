#ifndef ORBITALCAMERACONTROLLERCOMPONENT_H
#define ORBITALCAMERACONTROLLERCOMPONENT_H

#include <memory>

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "core/enableable.h"
#include "types/vec3.h"
#include "window/inputhandler.h"

/**
 * @class OrbitalCameraControllerComponent
 * @brief Component that handles orbital camera movement around a target.
 */
class OrbitalCameraControllerComponent : public BaseComponent, public Enableable {
public:
    OrbitalCameraControllerComponent() = default;

    /**
     * @brief Constructor for OrbitalCameraControllerComponent.
     * @param inputHandler Pointer to the input handler.
     * @param maxPitch Maximum pitch angle.
     * @param minPitch Minimum pitch angle.
     * @param lookSensitivity Look sensitivity.
     * @param orbitTarget The target point to orbit around.
     * @param orbitRadius The radius of the orbit.
     */
    OrbitalCameraControllerComponent(InputHandler *inputHandler, float maxPitch, float minPitch,
                                     float lookSensitivity, Vec3 orbitTarget, float orbitRadius);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    void onUpdate(float dt) override;

private:
    InputHandler *inputHandler_ = nullptr;
    float maxPitch_ = 89.0f;
    float minPitch_ = 0;
    float lookSensitivity_ = 50.0f;

    float pitch_ = 0.0f;
    float yaw_ = 180.0f;

    Vec3 orbitTarget_;
    float orbitRadius_ = 5;
};

#endif