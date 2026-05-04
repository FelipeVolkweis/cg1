#include "vehiclecomponent.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "components/meshcomponent.h"
#include "core/node.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"

VehicleComponent::VehicleComponent(PhysicsEngine *physicsEngine, float mass)
    : physicsEngine_(physicsEngine), initialMass_(mass) {}

void VehicleComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                            InputHandler &inputHandler) {
    physicsEngine_ = &physicsEngine;
    if (data["mass"])
        initialMass_ = data["mass"].as<float>();

    if (data["wheels"] && data["wheels"].IsSequence()) {
        for (auto wheelData : data["wheels"]) {
            ParsedWheelData pwd;
            if (wheelData["node"])
                pwd.nodeName = wheelData["node"].as<std::string>();

            pwd.connectionPoint = btVector3(0, 0, 0);
            if (wheelData["connectionPoint"] && wheelData["connectionPoint"].IsSequence()) {
                pwd.connectionPoint = btVector3(wheelData["connectionPoint"][0].as<float>(),
                                                wheelData["connectionPoint"][1].as<float>(),
                                                wheelData["connectionPoint"][2].as<float>());
            }

            pwd.suspensionRestLength = 0.6f;
            if (wheelData["suspensionRestLength"])
                pwd.suspensionRestLength = wheelData["suspensionRestLength"].as<float>();

            pwd.radius = 0.3f;
            if (wheelData["radius"])
                pwd.radius = wheelData["radius"].as<float>();

            pwd.isFrontWheel = false;
            if (wheelData["isFrontWheel"])
                pwd.isFrontWheel = wheelData["isFrontWheel"].as<bool>();

            parsedWheels_.push_back(pwd);
        }
    }
}

void VehicleComponent::addWheel(std::shared_ptr<Node> wheelNode,
                                const btVector3 &connectionPointCS0, float suspensionRestLength,
                                float wheelRadius, bool isFrontWheel) {
    wheelsToInitialize_.push_back(
        {wheelNode, connectionPointCS0, suspensionRestLength, wheelRadius, isFrontWheel});
}

bool VehicleComponent::onStart() {
    auto node = getNode();
    if (!node)
        return false;

    const BaseShape *shape = nullptr;
    auto meshComp = node->getComponent<MeshComponent>();
    if (meshComp) {
        shape = meshComp->getShape();
    }

    if (!shape) {
        Logger::Warn("VehicleComponent started without a BaseShape (from MeshComponent) on node: ",
                     node->getName());
        return false;
    }

    vehicle_ =
        std::make_unique<Vehicle>(physicsEngine_, initialMass_, node->getTransformation(), shape);

    vehicle_->addToWorld(physicsEngine_->getWorld());

    // Resolve parsed wheels
    if (!parsedWheels_.empty()) {
        auto root = node;
        while (root->getParent()) {
            root = root->getParent();
        }

        for (const auto &pwd : parsedWheels_) {
            auto wNode = Node::findNodeByName(root, pwd.nodeName);
            if (wNode) {
                addWheel(wNode, pwd.connectionPoint, pwd.suspensionRestLength, pwd.radius,
                         pwd.isFrontWheel);
            } else {
                Logger::Warn("VehicleComponent could not find wheel node: ", pwd.nodeName);
            }
        }
        parsedWheels_.clear();
    }

    for (const auto &w : wheelsToInitialize_) {
        vehicle_->addWheel(w.connectionPoint, w.suspensionRestLength, w.radius, w.isFrontWheel);
        activeWheelNodes_.push_back({w.node, w.node->getTransformation()});
    }
    wheelsToInitialize_.clear();

    return true;
}

void VehicleComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node || !vehicle_)
        return;

    node->setTransform(vehicle_->getChassisTransform());

    for (size_t i = 0; i < activeWheelNodes_.size(); ++i) {
        if (activeWheelNodes_[i].node) {
            Transformation t = vehicle_->getWheelTransform(i);
            activeWheelNodes_[i].node->setTransform(t * activeWheelNodes_[i].offset);
        }
    }
}

bool VehicleComponent::onEnd() {
    if (vehicle_ && physicsEngine_) {
        vehicle_->removeFromWorld(physicsEngine_->getWorld());
    }
    return true;
}
