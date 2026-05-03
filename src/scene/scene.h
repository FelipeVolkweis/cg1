#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera/camera.h"
#include "core/node.h"
#include "physics/physicsengine.h"
#include "renderer/renderer.h"
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

    void start();
    /**
     * @brief Updates the physics and components in the scene.
     * @param dt Time step since the last frame.
     */
    void update(float dt);

    void end();

    /**
     * @brief Gets the physics engine managing this scene.
     */
    PhysicsEngine *getPhysicsEngine() {
        return &physicsEngine_;
    }

    /**
     * @brief Populates the renderer with shapes from the scene and its subscenes.
     * @param renderer The renderer to populate.
     */
    void populateRenderer(Renderer &renderer);

    std::shared_ptr<Camera> getActiveCamera();

    const std::vector<std::shared_ptr<Camera>> &getCameras() const {
        return cameras_;
    }

    std::shared_ptr<Node> getRoot() const {
        return root_;
    }

private:
    std::shared_ptr<Node> root_;
    PhysicsEngine physicsEngine_;
    std::vector<std::shared_ptr<Camera>> cameras_;
};

#endif