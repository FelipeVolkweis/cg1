#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <memory>

#include <btBulletDynamicsCommon.h>

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    bool initialize();

    void stepSimulation(float deltaTime);

    btDiscreteDynamicsWorld *getWorld() {
        return dynamicsWorld_.get();
    }

private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_;
    std::unique_ptr<btCollisionDispatcher> dispatcher_;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache_;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
};

#endif