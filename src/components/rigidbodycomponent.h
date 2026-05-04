#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <iosfwd>
#include <memory>

#include "core/component.h"
#include "physics/physicsengine.h"
#include "physics/rigidbody.h"

/**
 * @class RigidBodyComponent
 * @brief Component that adds rigid body physics properties to a Node.
 */
class RigidBodyComponent : public BaseComponent {
public:
    RigidBodyComponent() = default;

    /**
     * @brief Constructor for RigidBodyComponent.
     * @param physicsEngine Pointer to the physics engine.
     * @param mass Mass of the rigid body.
     */
    RigidBodyComponent(PhysicsEngine *physicsEngine, float mass);
    ~RigidBodyComponent() override;

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    bool onStart() override;
    void onUpdate(float dt) override;
    bool onEnd() override;

    RigidBody *getRigidBody() const {
        return rigidBody_.get();
    }

private:
    PhysicsEngine *physicsEngine_ = nullptr;
    std::unique_ptr<RigidBody> rigidBody_;

    float initialMass_ = 0.0f;
};

#endif
