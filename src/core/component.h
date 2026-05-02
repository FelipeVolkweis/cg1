#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class Node;

class BaseComponent {
public:
    virtual ~BaseComponent() = default;

    void setNode(std::weak_ptr<Node> node) {
        node_ = node;
    }

    std::shared_ptr<Node> getNode() const {
        return node_.lock();
    }

    virtual bool onStart() {
        return true;
    }

    virtual void onPreUpdate() {};
    virtual void onUpdate() {};
    virtual void onPostUpdate() {};

    virtual bool onEnd() {
        return true;
    }

protected:
    std::weak_ptr<Node> node_;
};

#endif