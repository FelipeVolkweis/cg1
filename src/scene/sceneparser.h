#ifndef YAMLSCENEPARSER_H
#define YAMLSCENEPARSER_H

#include <memory>
#include <string>

#include "core/node.h"

class PhysicsEngine;
struct InputHandler;

namespace YAML {
class Node;
}

class SceneParser {
public:
    static bool load(const std::string &filepath, std::shared_ptr<Node> root,
                     PhysicsEngine &physicsEngine, InputHandler &inputHandler);

private:
    static std::shared_ptr<Node> parseNode(const YAML::Node &nodeData, PhysicsEngine &physicsEngine,
                                           InputHandler &inputHandler);
};

#endif
