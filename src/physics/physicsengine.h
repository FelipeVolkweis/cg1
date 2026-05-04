#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <memory>

#include <btBulletDynamicsCommon.h>

/**
 * @brief Class that manages the physics simulation using the Bullet Physics library.
 */
class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    /**
     * @brief Initializes the physics engine components.
     *
     * @return true If initialization was successful.
     * @return false Otherwise.
     */
    bool initialize();

    /**
     * @brief Advances the physics simulation by the given time step.
     *
     * @param deltaTime The time step for the simulation.
     */
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
