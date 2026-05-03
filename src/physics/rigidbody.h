#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "transformations/transformation.h"

class BaseShape;

class RigidBody {
public:
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
