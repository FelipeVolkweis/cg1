#include "vehiclecomponent.h"

#include "core/node.h"
#include "transformations/rotation.h"

VehicleComponent::VehicleComponent(PhysicsEngine *physicsEngine, float mass,
                                   std::unique_ptr<btCollisionShape> shape)
    : physicsEngine_(physicsEngine), initialMass_(mass), initialShape_(std::move(shape)) {}

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

    vehicle_ = std::make_unique<Vehicle>(physicsEngine_, initialMass_, node->getTransformation(),
                                         std::move(initialShape_));

    vehicle_->addToWorld(physicsEngine_->getWorld());

    for (const auto &w : wheelsToInitialize_) {
        vehicle_->addWheel(w.connectionPoint, w.suspensionRestLength, w.radius, w.isFrontWheel);
        activeWheelNodes_.push_back(w.node);
    }
    wheelsToInitialize_.clear();

    return true;
}

void VehicleComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node || !vehicle_)
        return;

    node->setTransform(vehicle_->getChassisTransform());

    static Transformation wheelOffset = RotationY(90.0f); // TODO: move this to a param if needed

    for (size_t i = 0; i < activeWheelNodes_.size(); ++i) {
        if (activeWheelNodes_[i]) {
            Transformation t = vehicle_->getWheelTransform(i);
            activeWheelNodes_[i]->setTransform(t * wheelOffset);
        }
    }
}

bool VehicleComponent::onEnd() {
    if (vehicle_ && physicsEngine_) {
        vehicle_->removeFromWorld(physicsEngine_->getWorld());
    }
    return true;
}
