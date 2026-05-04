#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera/camera.h"
#include "components/cameracomponent.h"
#include "core/node.h"
#include "physics/physicsengine.h"
#include "renderer/renderable.h"
#include "renderer/renderer.h"
#include "renderer/skybox.h"
#include "shapes/baseshape.h"
#include "transformations/transformation.h"

/**
 * @class Scene
 * @brief Manages a collection of shapes and subscenes, handling loading and rendering.
 */
class Scene {
public:
    Scene(std::shared_ptr<Node> root) : root_(root) {
        physicsEngine_.initialize();
    };

    /**
     * @brief Starts the scene and its components.
     */
    void start();

    /**
     * @brief Updates the physics and components in the scene.
     * @param dt Time step since the last frame.
     */
    void update(float dt);

    /**
     * @brief Finalizes the scene and its components.
     */
    void end();

    PhysicsEngine &getPhysicsEngine() {
        return physicsEngine_;
    }

    void setSkybox(std::shared_ptr<Skybox> skybox) {
        skybox_ = skybox;
    }

    std::shared_ptr<Skybox> getSkybox() const {
        return skybox_;
    }

    /**
     * @brief Populates the renderer with shapes from the scene and its subscenes.
     * @param renderer The renderer to populate.
     */
    void populateRenderer(Renderer &renderer);

    /**
     * @brief Gets the currently active camera in the scene.
     * @return Shared pointer to the active camera.
     */
    std::shared_ptr<Camera> getActiveCamera();

    /**
     * @brief Gets the root node of the scene hierarchy.
     * @return Shared pointer to the root node.
     */
    std::shared_ptr<Node> getRoot() const {
        return root_;
    }

private:
    std::shared_ptr<Node> root_;
    PhysicsEngine physicsEngine_;
    std::vector<std::shared_ptr<CameraComponent>> cameras_;
    std::shared_ptr<Skybox> skybox_;
};

#endif
