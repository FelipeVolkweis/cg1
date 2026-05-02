#include "carcontrollercomponent.h"

#include <algorithm>

#include "core/node.h"

CarControllerComponent::CarControllerComponent(InputHandler *inputHandler)
    : inputHandler_(inputHandler) {}

void CarControllerComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node || !inputHandler_)
        return;

    auto vehicleComp = node->getComponent<VehicleComponent>();
    if (!vehicleComp || !vehicleComp->getVehicle())
        return;

    auto car = vehicleComp->getVehicle();

    float throttleInput =
        inputHandler_->getYAxisInputVector().x() - inputHandler_->getYAxisInputVector().y();

    car->applyEngineForce(throttleInput * maxEngineForce_, 2);
    car->applyEngineForce(throttleInput * maxEngineForce_, 3);

    float steerInput =
        inputHandler_->getXAxisInputVector().x() - inputHandler_->getXAxisInputVector().y();

    if (steerInput != 0.0f) {
        currentSteering_ += steerInput * steeringSensitivity_ * dt;
    } else {
        if (currentSteering_ > 0.0f) {
            currentSteering_ -= steeringSensitivity_ * dt;
            if (currentSteering_ < 0.0f)
                currentSteering_ = 0.0f;
        } else if (currentSteering_ < 0.0f) {
            currentSteering_ += steeringSensitivity_ * dt;
            if (currentSteering_ > 0.0f)
                currentSteering_ = 0.0f;
        }
    }

    currentSteering_ = std::clamp(currentSteering_, -1.0f, 1.0f);

    float finalSteerAngle = currentSteering_ * maxSteeringAngle_;
    car->setSteeringValue(finalSteerAngle, 0);
    car->setSteeringValue(finalSteerAngle, 1);
}
