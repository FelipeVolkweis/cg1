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

    /**
     * @brief Checks if a renderable with the given ID exists in the renderer.
     * @param id The unique ID of the renderable.
     * @return true if it exists, false otherwise.
     */
    bool isRenderableHere(uint64_t id) {
        return renderables_.find(id) != renderables_.end();
    }

    /**
     * @brief Sets the transformation matrix for a specific renderable.
     * @param id The unique ID of the renderable.
     * @param transformation The transformation to apply.
     */
    void setRenderableTransformation(uint64_t id, const Transformation &transformation) {
        transforms_[id] = transformation;
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

    int viewLocation_;
    int projectionLocation_;

    int viewPosLocation_;

    // DirectionalLight Locations Cache
    // begin
    int dl_directionLocation_;
    int dl_ambientLocation_;
    int dl_diffuseLocation_;
    int dl_specularLocation_;
    // end
    int pointLightsLocation_;
    int spolightsLocation_;

    int numPointLightsLocation_;
    int numSpotlightsLocation_;

    /**
     * @brief Compiles a shader from source.
     * @param type The type of shader (e.g., GL_VERTEX_SHADER).
     * @param source The shader source code.
     * @return The compiled shader handle, or 0 on failure.
     */
    uint32_t compileShader(uint32_t type, const char *source);
};

#endif
