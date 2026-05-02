#include "physicsengine.h"

PhysicsEngine::PhysicsEngine() = default;

PhysicsEngine::~PhysicsEngine() {}

bool PhysicsEngine::initialize() {
    collisionConfiguration_ = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher_ = std::make_unique<btCollisionDispatcher>(collisionConfiguration_.get());
    overlappingPairCache_ = std::make_unique<btDbvtBroadphase>();
    solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();

    dynamicsWorld_ =
        std::make_unique<btDiscreteDynamicsWorld>(dispatcher_.get(), overlappingPairCache_.get(),
                                                  solver_.get(), collisionConfiguration_.get());
    dynamicsWorld_->setGravity(btVector3(0, -9.81f, 0));

    return true;
}

void PhysicsEngine::stepSimulation(float deltaTime) {
    if (dynamicsWorld_) {
        dynamicsWorld_->stepSimulation(deltaTime, 10);
    }
}