#ifndef SCENE_H
#define SCENE_H

#include "renderer/renderer.h"
#include "shapes/baseshape.h"
#include "transformations/transformation.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class Scene
 * @brief Manages a collection of shapes and subscenes, handling loading and rendering.
 */
class Scene {
public:
    struct Subscene {
        std::unique_ptr<Scene> scene;
        Transformation transform;
        std::vector<std::string> modifiers;
    };

    Scene() = default;

    /**
     * @brief Loads a scene from a file.
     * @param filepath Path to the scene file.
     * @return true if loading was successful, false otherwise.
     */
    bool load(const std::string &filepath);

    /**
     * @brief Populates the renderer with shapes from the scene and its subscenes.
     * @param renderer The renderer to populate.
     * @param modifierTransforms Map of dynamic modifier names to their transformations.
     * @param parentTransform The transformation inherited from a parent scene.
     */
    void
    populateRenderer(Renderer &renderer,
                     const std::unordered_map<std::string, Transformation> &modifierTransforms = {},
                     const Transformation &parentTransform = Transformation());

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
    void parseSubscene(std::istringstream &iss);

    /**
     * @brief Applies dynamic modifiers to a world transformation.
     * @param world The transformation to be modified.
     * @param modifiers List of modifier names to apply.
     * @param modifierTransforms Map of modifier names to their current transformations.
     */
    void applyDynamicModifiers(
        Transformation &world, const std::vector<std::string> &modifiers,
        const std::unordered_map<std::string, Transformation> &modifierTransforms) const;

    std::vector<std::unique_ptr<BaseShape>> shapes_;
    std::vector<Subscene> subscenes_;
};

#endif