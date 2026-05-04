#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>

#include "component.h"
#include "transformations/transformation.h"

class Node : public std::enable_shared_from_this<Node> {
public:
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

    template <typename T> std::shared_ptr<T> getComponent() {
        for (auto &comp : components_) {
            auto casted = std::dynamic_pointer_cast<T>(comp);
            if (casted)
                return casted;
        }

        return nullptr;
    }

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

private:
    std::string name_;
    std::vector<std::shared_ptr<BaseComponent>> components_;
    std::weak_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    Transformation transform_;
};

#endif