#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "camera/camera.h"
#include "lights/renderablelight.h"
#include "math/transformations/transformation.h"
#include "meshes/renderablemesh.h"
#include "renderer/lights/directionallight.h"
#include "renderer/lights/spotlight.h"
#include "renderer/shader/shader.h"
#include "renderer/skybox.h"

struct GLFWwindow;

/**
 * @class Renderer
 * @brief Handles the rendering of shapes using OpenGL.
 */
class Renderer {
public:
    Renderer();

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

    void addRenderable(uint64_t id, std::shared_ptr<RenderableMesh> renderable);

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

    void setDirectionalLight(std::shared_ptr<RenderableDirectionalLight> directionalLight);
    void addPointLight(uint64_t id, std::shared_ptr<RenderablePointLight> pointLight);
    void addSpotlight(uint64_t id, std::shared_ptr<RenderableSpotlight> spotlight);

    void setWindow(GLFWwindow *window) {
        window_ = window;
    }

private:
    GLFWwindow *window_;

    std::shared_ptr<Shader> mainShaderProgram_;
    std::shared_ptr<Shader> shadowShaderProgram_;

    std::weak_ptr<Camera> activeCamera_;
    std::shared_ptr<Skybox> skybox_;

    std::unordered_map<uint64_t, std::shared_ptr<RenderableMesh>> renderables_;
    std::unordered_map<uint64_t, Transformation> transforms_;

    std::shared_ptr<RenderableDirectionalLight> directionalLight_;
    std::unordered_map<uint64_t, std::shared_ptr<RenderablePointLight>> pointLights_;
    std::unordered_map<uint64_t, std::shared_ptr<RenderableSpotlight>> spotlights_;

    void renderDirectionalLight();
    void renderPointLights();
    void renderSpotlights();
    void renderOpaqueMeshes();
    void renderTransluscenteMeshes();
};

#endif
