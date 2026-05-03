#ifndef MESH_H
#define MESH_H

#include <memory>

#include "core/component.h"
#include "shapes/baseshape.h"

class MeshComponent : public BaseComponent {
public:
    MeshComponent() = default;

    MeshComponent(std::unique_ptr<BaseShape> shape) : shape_(std::move(shape)) {}

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    const BaseShape *getShape() {
        return shape_.get();
    }

private:
    std::unique_ptr<BaseShape> shape_;
};

#endif