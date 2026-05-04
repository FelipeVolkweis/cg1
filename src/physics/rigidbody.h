#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "transformations/transformation.h"

class BaseShape;

/**
 * @brief Class representing a rigid body in the physics simulation.
 */
class RigidBody {
public:
    /**
     * @brief Constructs a new RigidBody object.
     *
     * @param mass The mass of the rigid body.
     * @param initialTransform The initial transformation of the rigid body.
     * @param shape The shape of the rigid body.
     */
    RigidBody(float mass, const Transformation &initialTransform, const BaseShape *shape);
    ~RigidBody();

    btRigidBody *getBulletRigidBody() const {
        return body_.get();
    }

    Transformation getTransform() const;

    void setTransform(const Transformation &transform);

private:
    std::unique_ptr<btCollisionShape> shape_;
    std::unique_ptr<btDefaultMotionState> motionState_;
    std::unique_ptr<btRigidBody> body_;
};

#endif
