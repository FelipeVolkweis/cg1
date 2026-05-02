#include "rigidbodycomponent.h"

#include "core/node.h"

RigidBodyComponent::RigidBodyComponent(PhysicsEngine *physicsEngine, float mass,
                                       std::unique_ptr<btCollisionShape> shape)
    : physicsEngine_(physicsEngine), initialMass_(mass), initialShape_(std::move(shape)) {}

RigidBodyComponent::~RigidBodyComponent() = default;

bool RigidBodyComponent::onStart() {
    auto node = getNode();
    if (!node)
        return false;

    rigidBody_ = std::make_unique<RigidBody>(initialMass_, node->getTransformation(),
                                             std::move(initialShape_));

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
