#ifndef NODE_H
#define NODE_H

#include <memory>
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

    std::vector<std::shared_ptr<BaseComponent>> getComponents() {
        return components_;
    }

    template <typename T> std::shared_ptr<T> getComponent() {
        for (auto &comp : components_) {
            auto casted = std::dynamic_pointer_cast<T>(comp);
            if (casted)
                return casted;
        }

        return nullptr;
    }

private:
    std::vector<std::shared_ptr<BaseComponent>> components_;
    std::weak_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    Transformation transform_;
};

#endif