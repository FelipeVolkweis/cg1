#ifndef RENDERER_H
#define RENDERER_H

#include "transformations/transformation.h"
#include <cstdint>
#include <vector>

class BaseShape;

struct RenderItem {
    const BaseShape *shape;
    Transformation transform;
};

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    bool initialize();
    void render();

    void addShape(const BaseShape *shape, const Transformation &transform) {
        items_.push_back({shape, transform});
    }

    void clear() {
        items_.clear();
    }

private:
    std::vector<RenderItem> items_;
    uint32_t shaderProgram_ = 0;
    uint32_t vao_ = 0;
    uint32_t vbo_ = 0;

    uint32_t compileShader(uint32_t type, const char *source);
};

#endif