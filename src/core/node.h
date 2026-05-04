#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>

#include "component.h"
#include "transformations/transformation.h"

/**
 * @class Node
 * @brief Represents a node in the scene graph.
 *
 * Nodes can have a parent, multiple children, and multiple components.
 * They also handle transformations in local and world space.
 */
class Node : public std::enable_shared_from_this<Node> {
public:
    /**
     * @brief Constructor for Node.
     * Initializes the node with a unique ID.
     */
    Node() : id_(lastId_++) {}

    void addChild(std::shared_ptr<Node> child) {
        if (child) {
            child->parent_ = shared_from_this();
            children_.push_back(child);
        }
    }

    void addComponent(std::shared_ptr<BaseComponent> component) {
        if (component) {
            component->setNode(shared_from_this());
            components_.push_back(component);
        }
    }

    void setTransform(const Transformation &transform) {
        transform_ = transform;
    }

    std::shared_ptr<Node> getParent() const {
        return parent_.lock();
    }

    const std::vector<std::shared_ptr<Node>> &getChildren() const {
        return children_;
    }

    Transformation &getTransformation() {
        return transform_;
    }

    /**
     * @brief Gets the world-space transformation of this node.
     * @return The world-space transformation.
     */
    Transformation getWorldTransform() const {
        Transformation world = transform_;
        auto p = parent_.lock();
        if (p) {
            world = p->getWorldTransform() * world;
        }
        return world;
    }

    std::vector<std::shared_ptr<BaseComponent>> getComponents() {
        return components_;
    }

    void setName(const std::string &name) {
        name_ = name;
    }

    const std::string &getName() const {
        return name_;
    }

    /**
     * @brief Gets a component of a specific type attached to this node.
     * @tparam T The type of the component to get.
     * @return A shared pointer to the component, or nullptr if not found.
     */
    template <typename T> std::shared_ptr<T> getComponent() {
        for (auto &comp : components_) {
            auto casted = std::dynamic_pointer_cast<T>(comp);
            if (casted)
                return casted;
        }

        return nullptr;
    }

    /**
     * @brief Finds a node by its name starting from a root node.
     * @param root The root node to start searching from.
     * @param name The name of the node to find.
     * @return A shared pointer to the found node, or nullptr if not found.
     */
    static std::shared_ptr<Node> findNodeByName(std::shared_ptr<Node> root,
                                                const std::string &name) {
        if (!root)
            return nullptr;
        if (root->getName() == name)
            return root;
        for (auto &child : root->getChildren()) {
            if (auto found = findNodeByName(child, name)) {
                return found;
            }
        }
        return nullptr;
    }

    /**
     * @brief Gets the unique ID of this node.
     * @return The unique ID.
     */
    uint64_t getId() {
        return id_;
    }

private:
    std::string name_;
    std::vector<std::shared_ptr<BaseComponent>> components_;
    std::weak_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    Transformation transform_;
    const uint64_t id_;

    static uint64_t lastId_;
};

#endif