#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "core/component.h"
#include "physics/physicsengine.h"
#include "physics/rigidbody.h"

class RigidBodyComponent : public BaseComponent {
public:
    RigidBodyComponent(PhysicsEngine *physicsEngine, float mass,
                       std::unique_ptr<btCollisionShape> shape);
    ~RigidBodyComponent() override;

    bool onStart() override;
    void onUpdate(float dt) override;
    bool onEnd() override;

    RigidBody *getRigidBody() const {
        return rigidBody_.get();
    }

private:
    PhysicsEngine *physicsEngine_;
    std::unique_ptr<RigidBody> rigidBody_;

    float initialMass_;
    std::unique_ptr<btCollisionShape> initialShape_;
};

#endif
