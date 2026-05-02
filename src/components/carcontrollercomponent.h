#ifndef CARCONTROLLERCOMPONENT_H
#define CARCONTROLLERCOMPONENT_H

#include "components/vehiclecomponent.h"
#include "core/component.h"
#include "window/inputhandler.h"

class CarControllerComponent : public BaseComponent {
public:
    CarControllerComponent(InputHandler *inputHandler);
    void onUpdate(float dt) override;

private:
    InputHandler *inputHandler_;
    float maxEngineForce_ = 2000.0f;
    float maxSteeringAngle_ = 0.5f;
    float steeringSensitivity_ = 3.0f;

    float currentSteering_ = 0.0f;
};

#endif
