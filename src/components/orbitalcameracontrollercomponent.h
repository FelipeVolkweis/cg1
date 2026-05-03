#ifndef ORBITALCAMERACONTROLLERCOMPONENT_H
#define ORBITALCAMERACONTROLLERCOMPONENT_H

#include <memory>

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "core/enableable.h"
#include "types/vec3.h"
#include "window/inputhandler.h"

class OrbitalCameraControllerComponent : public BaseComponent, public Enableable {
public:
    OrbitalCameraControllerComponent() = default;
    OrbitalCameraControllerComponent(InputHandler *inputHandler, float maxPitch, float maxYaw,
                                     float lookSensitivity, Vec3 orbitTarget, float orbitRadius);

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

    Vec3 orbitTarget_;
    float orbitRadius_ = 5;
};

#endif