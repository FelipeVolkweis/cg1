#ifndef MESH_H
#define MESH_H

#include <memory>

#include "core/component.h"
#include "shapes/baseshape.h"

/**
 * @class MeshComponent
 * @brief Component that represents a 3D mesh.
 */
class MeshComponent : public BaseComponent {
public:
    MeshComponent() = default;

    /**
     * @brief Constructor for MeshComponent with a shape.
     * @param shape Unique pointer to the BaseShape.
     */
    MeshComponent(std::unique_ptr<BaseShape> shape) : shape_(std::move(shape)) {}

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    const BaseShape *getShape() {
        return shape_.get();
    }

    std::shared_ptr<RenderableMesh> getRenderableMesh() {
        return renderableMesh_;
    }

private:
    std::unique_ptr<BaseShape> shape_;
    std::shared_ptr<RenderableMesh> renderableMesh_;
};

#endif