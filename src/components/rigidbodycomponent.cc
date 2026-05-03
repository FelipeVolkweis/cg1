#include "rigidbodycomponent.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "components/meshcomponent.h"
#include "core/node.h"
#include "utils/logger.h"

RigidBodyComponent::RigidBodyComponent(PhysicsEngine *physicsEngine, float mass)
    : physicsEngine_(physicsEngine), initialMass_(mass) {}

RigidBodyComponent::~RigidBodyComponent() = default;

void RigidBodyComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                              InputHandler &inputHandler) {
    physicsEngine_ = &physicsEngine;
    if (data["mass"])
        initialMass_ = data["mass"].as<float>();
}

bool RigidBodyComponent::onStart() {
    auto node = getNode();
    if (!node)
        return false;

    const BaseShape *shape = nullptr;
    auto meshComp = node->getComponent<MeshComponent>();
    if (meshComp) {
        shape = meshComp->getShape();
    }

    if (!shape) {
        Logger::Warn(
            "RigidBodyComponent started without a BaseShape (from MeshComponent) on node: ",
            node->getName());
        return false;
    }

    rigidBody_ = std::make_unique<RigidBody>(initialMass_, node->getTransformation(), shape);

    physicsEngine_->getWorld()->addRigidBody(rigidBody_->getBulletRigidBody());

    return true;
}

void RigidBodyComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node || !rigidBody_)
        return;

    if (initialMass_ > 0.0f) {
        Transformation t = rigidBody_->getTransform();
        node->setTransform(t);
    }
}

bool RigidBodyComponent::onEnd() {
    if (rigidBody_ && physicsEngine_) {
        physicsEngine_->getWorld()->removeRigidBody(rigidBody_->getBulletRigidBody());
    }
    return true;
}
