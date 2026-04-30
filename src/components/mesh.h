#ifndef MESH_H
#define MESH_H

#include <memory>

#include "core/component.h"
#include "shapes/baseshape.h"

class Mesh : public BaseComponent {
public:
    Mesh(std::unique_ptr<BaseShape> shape) : shape_(std::move(shape)) {}

    const BaseShape *getShape() {
        return shape_.get();
    }

private:
    std::unique_ptr<BaseShape> shape_;
};

#endif