#ifndef CARCONTROLLERCOMPONENT_H
#define CARCONTROLLERCOMPONENT_H

#include <iosfwd>
#include <memory>

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "window/inputhandler.h"

class CarControllerComponent : public BaseComponent {
public:
    CarControllerComponent() = default;
    CarControllerComponent(InputHandler *inputHandler, float maxEngineForce = 2000.0f,
                           float maxSteeringAngle = 0.5f, float steeringSensitivity = 3.0f, float breakForce = 100.0f);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    void onUpdate(float dt) override;

    float getCurrentSteering() const {
        return currentSteering_;
    }

    float getMaxSteeringAngle() const {
        return maxSteeringAngle_;
    }

private:
    InputHandler *inputHandler_ = nullptr;
    float maxEngineForce_ = 2000.0f;
    float maxSteeringAngle_ = 0.5f;
    float steeringSensitivity_ = 3.0f;
    float brakeForce_ = 100.0f;

    float currentSteering_ = 0.0f;
};

#endif
