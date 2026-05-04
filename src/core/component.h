#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class PhysicsEngine;
struct InputHandler;

namespace YAML {
class Node;
}

class Node;

/**
 * @class BaseComponent
 * @brief Abstract base class for all components that can be attached to a Node.
 */
class BaseComponent {
public:
    virtual ~BaseComponent() = default;

    void setNode(std::weak_ptr<Node> node) {
        node_ = node;
    }

    std::shared_ptr<Node> getNode() const {
        return node_.lock();
    }

    /**
     * @brief Called when the component is started.
     * @return True if start was successful, false otherwise.
     */
    virtual bool onStart() {
        return true;
    }

    /**
     * @brief Called before the update step.
     */
    virtual void onPreUpdate() {};

    /**
     * @brief Called during the update step.
     * @param dt Delta time since last update.
     */
    virtual void onUpdate(float dt) {};

    /**
     * @brief Called after the update step.
     */
    virtual void onPostUpdate() {};

    /**
     * @brief Called when the component is ended.
     * @return True if end was successful, false otherwise.
     */
    virtual bool onEnd() {
        return true;
    }

    /**
     * @brief Loads component data from a YAML node.
     * @param data The YAML node containing component data.
     * @param physicsEngine Reference to the physics engine.
     * @param inputHandler Reference to the input handler.
     */
    virtual void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                      InputHandler &inputHandler) {}

protected:
    std::weak_ptr<Node> node_;
};

#endif