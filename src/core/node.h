#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>

#include "component.h"
#include "math/transformations/transformation.h"

#define UNNAMED_NODE "104ab42f11" // arbitrary hash

/**
 * @class Node
 * @brief Represents a node in the scene graph.
 *
 * Nodes can have a parent, multiple children, and multiple components.
 * They also handle transformations in local and world space.
 */
class Node : public std::enable_shared_from_this<Node> {
public:
    ~Node() {
        idToNode_.erase(id_);

        if (name_ != UNNAMED_NODE) {
            auto it = nameToId_.find(name_);
            if (it != nameToId_.end() && it->second == id_) {
                nameToId_.erase(it);
            }
        }
    }

    static std::shared_ptr<Node> create(const std::string &name = UNNAMED_NODE) {
        std::shared_ptr<Node> node(name == UNNAMED_NODE ? new Node() : new Node(name));

        idToNode_[node->getId()] = node;

        if (name != UNNAMED_NODE) {
            nameToId_[name] = node->getId();
        }

        return node;
    }

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

    const std::vector<std::shared_ptr<BaseComponent>> &getComponents() const {
        return components_;
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

    static std::shared_ptr<Node> findNodeByName(const std::string &name) {
        uint64_t id = -1;
        if (name == UNNAMED_NODE)
            return nullptr;

        if (nameToId_.count(name) > 0) {
            id = nameToId_[name];
        } else {
            return nullptr;
        }

        return findNodeById(id);
    }

    static std::shared_ptr<Node> findNodeById(uint64_t id) {
        if (id == -1)
            return nullptr;

        if (idToNode_.count(id) > 0) {
            return idToNode_[id].lock();
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
    /**
     * @brief Constructor for Node.
     * Initializes the node with a unique ID.
     */
    Node() : id_(lastId_++), name_(UNNAMED_NODE) {}

    Node(const std::string &name) : id_(lastId_++), name_(name) {}

    std::string name_;
    std::vector<std::shared_ptr<BaseComponent>> components_;
    std::weak_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    Transformation transform_;
    const uint64_t id_;

    static uint64_t lastId_;
    static std::unordered_map<std::string, uint64_t> nameToId_;
    static std::unordered_map<uint64_t, std::weak_ptr<Node>> idToNode_;
};

#endif