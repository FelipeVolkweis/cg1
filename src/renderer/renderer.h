#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "camera/camera.h"
#include "renderable.h"
#include "renderer/skybox.h"
#include "transformations/transformation.h"

/**
 * @class Renderer
 * @brief Handles the rendering of shapes using OpenGL.
 */
class Renderer {
public:
    Renderer() = default;

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

    void addRenderable(Renderable renderable);

    bool isRenderableHere(uint64_t id) {
        return renderables_.find(id) != renderables_.end();
    }

    void setRenderableTransformation(uint64_t id, const Transformation &tranformation) {
        transforms_[id] = tranformation;
    }

    void setSkybox(std::shared_ptr<Skybox> skybox) {
        skybox_ = skybox;
    }

private:
    std::weak_ptr<Camera> activeCamera_;
    std::shared_ptr<Skybox> skybox_;
    uint32_t shaderProgram_ = 0;

    std::unordered_map<uint64_t, Renderable> renderables_;
    std::unordered_map<uint64_t, Transformation> transforms_;

    /**
     * @brief Compiles a shader from source.
     * @param type The type of shader (e.g., GL_VERTEX_SHADER).
     * @param source The shader source code.
     * @return The compiled shader handle, or 0 on failure.
     */
    uint32_t compileShader(uint32_t type, const char *source);
};

#endif
