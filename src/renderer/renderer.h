#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "camera/camera.h"
#include "transformations/transformation.h"

class BaseShape;

/**
 * @struct RenderItem
 * @brief Represents a shape and its transformation to be rendered.
 */
struct RenderItem {
    const BaseShape *shape;
    Transformation transform;
};

/**
 * @class Renderer
 * @brief Handles the rendering of shapes using OpenGL.
 */
class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    /**
     * @brief Initializes the OpenGL state and compiles shaders.
     * @return true if initialization was successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Renders all added shapes to the current frame buffer.
     */
    void render();

    void setActiveCamera(std::weak_ptr<Camera> camera) {
        activeCamera_ = camera;
    }

    /**
     * @brief Adds a shape to the rendering queue.
     * @param shape Pointer to the shape to render.
     * @param transform Transformation to apply to the shape.
     */
    void addShape(const BaseShape *shape, const Transformation &transform) {
        items_.push_back({shape, transform});
    }

    /**
     * @brief Clears the rendering queue.
     */
    void clear() {
        items_.clear();
    }

private:
    std::vector<RenderItem> items_;
    std::weak_ptr<Camera> activeCamera_;
    uint32_t shaderProgram_ = 0;
    uint32_t vao_ = 0;
    uint32_t vbo_ = 0;

    /**
     * @brief Compiles a shader from source.
     * @param type The type of shader (e.g., GL_VERTEX_SHADER).
     * @param source The shader source code.
     * @return The compiled shader handle, or 0 on failure.
     */
    uint32_t compileShader(uint32_t type, const char *source);
};

#endif