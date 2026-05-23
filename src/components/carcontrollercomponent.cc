#include "carcontrollercomponent.h"

#include <algorithm>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/node.h"
#include "utils/deg2rad.h"
#include "window/inputhandler.h"
#include "window/window.h"

CarControllerComponent::CarControllerComponent(float maxEngineForce, float maxSteeringAngle,
                                               float steeringSensitivity, float brakeForce)
    : maxEngineForce_(maxEngineForce), maxSteeringAngle_(maxSteeringAngle),
      steeringSensitivity_(steeringSensitivity), brakeForce_(brakeForce) {}

void CarControllerComponent::load(const YAML::Node &data) {
    if (data["force"])
        maxEngineForce_ = data["force"].as<float>();
    if (data["angle"])
        maxSteeringAngle_ = data["angle"].as<float>() * DEG2RAD;
    if (data["sensitivity"])
        steeringSensitivity_ = data["sensitivity"].as<float>();
    if (data["brake"])
        brakeForce_ = data["brake"].as<float>();
}

void CarControllerComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node)
        return;

    auto vehicleComp = node->getComponent<VehicleComponent>();
    if (!vehicleComp || !vehicleComp->getVehicle())
        return;

    auto car = vehicleComp->getVehicle();

    auto &inputHandler = Window::instance().getInputHandler();

    float throttleInput =
        inputHandler.getMainYAxisInputVector().x() - inputHandler.getMainYAxisInputVector().y();

    car->applyEngineForce(throttleInput * maxEngineForce_, 2);
    car->applyEngineForce(throttleInput * maxEngineForce_, 3);

    float steerInput =
        inputHandler.getMainXAxisInputVector().x() - inputHandler.getMainXAxisInputVector().y();

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

    if (inputHandler.isSpaceBarHeldDown()) {
        for (int i = 0; i < 4; i++) {
            car->setBrake(brakeForce_, i);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            car->setBrake(0, i);
        }
    }
}
