#ifndef YAMLSCENEPARSER_H
#define YAMLSCENEPARSER_H

#include <memory>
#include <string>

#include "scene/scene.h"

struct InputHandler;

namespace YAML {
class Node;
}

class SceneParser {
public:
    static bool load(const std::string &filepath, std::shared_ptr<Scene> scene,
                     InputHandler &inputHandler);

private:
    static std::shared_ptr<Node> parseNode(const YAML::Node &nodeData, PhysicsEngine &physicsEngine,
                                           InputHandler &inputHandler);
};

#endif
