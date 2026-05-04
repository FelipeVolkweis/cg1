#include "steeringanimationcomponent.h"

#include <algorithm>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "carcontrollercomponent.h"
#include "core/node.h"
#include "transformations/rotation.h"
#include "utils/deg2rad.h"

void SteeringAnimationComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                                      InputHandler &inputHandler) {
    if (data["maxRotation"]) {
        maxRotation_ = data["maxRotation"].as<float>();
    }
    if (data["steeringNodeName"]) {
        steeringNodeName_ = data["steeringNodeName"].as<std::string>();
    }
}

bool SteeringAnimationComponent::onStart() {
    auto node = getNode();
    if (!node)
        return false;

    auto steer = Node::findNodeByName(node, steeringNodeName_);
    if (steer) {
        startingTransformation_ = steer->getTransformation();
    } else {
        return false;
    }
    return true;
}

void SteeringAnimationComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node)
        return;

    auto steer = Node::findNodeByName(node, steeringNodeName_);
    if (!steer)
        return;

    auto controller = node->getComponent<CarControllerComponent>();
    if (!controller)
        return;

    float steering = controller->getCurrentSteering();
    float angle = steering * maxRotation_;

    steer->setTransform(startingTransformation_ * RotationY(angle));
}
