#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <memory>
#include <sstream>
#include <string>

#include "core/node.h"
#include "shapes/baseshape.h"

class SceneParser {
public:
    /**
     * @brief Loads a scene from a file and populates the given root node.
     * @param filepath Path to the scene file.
     * @param root The root node to populate.
     * @return true if loading was successful, false otherwise.
     */
    static bool load(const std::string &filepath, std::shared_ptr<Node> root);

private:
    /**
     * @brief Factory method to create a shape by name.
     * @param name The name of the shape (e.g., "box", "sphere").
     * @return Unique pointer to the created shape, or nullptr if unknown.
     */
    static std::unique_ptr<BaseShape> makeShape(const std::string &name);

    /**
     * @brief Parses a subscene entry from a stream.
     * @param iss The input string stream containing subscene data.
     */
    static std::shared_ptr<Node> parseSubscene(std::istringstream &iss);
};

#endif