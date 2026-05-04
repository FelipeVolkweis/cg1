#ifndef YAMLSCENEPARSER_H
#define YAMLSCENEPARSER_H

#include <memory>
#include <string>

#include "scene/scene.h"

struct InputHandler;

namespace YAML {
class Node;
}

/**
 * @class SceneParser
 * @brief Provides functionality to load a scene from a YAML file.
 */
class SceneParser {
public:
    /**
     * @brief Loads a scene from a YAML file.
     * @param filepath The path to the YAML file.
     * @param scene Shared pointer to the scene to be populated.
     * @param inputHandler Reference to the input handler.
     * @return true if loading was successful, false otherwise.
     */
    static bool load(const std::string &filepath, std::shared_ptr<Scene> scene,
                     InputHandler &inputHandler);

private:
    /**
     * @brief Parses a single YAML node and creates a corresponding scene node.
     * @param nodeData The YAML node data.
     * @param physicsEngine Reference to the physics engine.
     * @param inputHandler Reference to the input handler.
     * @return Shared pointer to the created scene node.
     */
    static std::shared_ptr<Node> parseNode(const YAML::Node &nodeData, PhysicsEngine &physicsEngine,
                                           InputHandler &inputHandler);
};

#endif
