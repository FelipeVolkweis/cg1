#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

    /**
     * @brief Loads a scene from a file.
     * @param filepath Path to the scene file.
     * @return true if loading was successful, false otherwise.
     */
    bool load(const std::string &filepath, std::shared_ptr<Node> root);
    bool load(const std::string &filepath);

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

private:
    /**
     * @brief Factory method to create a shape by name.
     * @param name The name of the shape (e.g., "box", "sphere").
     * @return Unique pointer to the created shape, or nullptr if unknown.
     */
    std::unique_ptr<BaseShape> makeShape(const std::string &name);

    /**
     * @brief Parses a subscene entry from a stream.
     * @param iss The input string stream containing subscene data.
     */
    std::shared_ptr<Node> parseSubscene(std::istringstream &iss);

    std::shared_ptr<Node> root_;
    PhysicsEngine physicsEngine_;
};

#endif