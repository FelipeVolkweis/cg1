#include "sceneparser.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/componentfactory.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"

bool SceneParser::load(const std::string &filepath, std::shared_ptr<Node> root,
                       PhysicsEngine &physicsEngine, InputHandler &inputHandler) {
    YAML::Node config = YAML::LoadFile(filepath);
    if (config["root"]) {
        auto rootNode = parseNode(config["root"], physicsEngine, inputHandler);
        if (rootNode) {
            for (auto &child : rootNode->getChildren()) {
                root->addChild(child);
            }
            for (auto &comp : rootNode->getComponents()) {
                root->addComponent(comp);
            }
            root->setName(rootNode->getName());
            root->setTransform(rootNode->getTransformation());
        }
        return true;
    }

    Logger::Error("YAML scene file missing 'root' node: ", filepath);
    return false;
}

std::shared_ptr<Node> SceneParser::parseNode(const YAML::Node &nodeData,
                                             PhysicsEngine &physicsEngine,
                                             InputHandler &inputHandler) {
    auto node = std::make_shared<Node>();

    if (nodeData["name"]) {
        node->setName(nodeData["name"].as<std::string>());
    }

    Vec3 origin(0, 0, 0);
    if (nodeData["origin"] && nodeData["origin"].IsSequence()) {
        origin = Vec3(nodeData["origin"][0].as<float>(), nodeData["origin"][1].as<float>(),
                      nodeData["origin"][2].as<float>());
    }

    Vec3 rotation(0, 0, 0);
    if (nodeData["rotation"] && nodeData["rotation"].IsSequence()) {
        rotation = Vec3(nodeData["rotation"][0].as<float>(), nodeData["rotation"][1].as<float>(),
                        nodeData["rotation"][2].as<float>());
    }

    Vec3 scale(1, 1, 1);
    if (nodeData["scale"] && nodeData["scale"].IsSequence()) {
        scale = Vec3(nodeData["scale"][0].as<float>(), nodeData["scale"][1].as<float>(),
                     nodeData["scale"][2].as<float>());
    }

    Transformation transform = Translation(origin) * RotationZ(rotation.z()) *
                               RotationY(rotation.y()) * RotationX(rotation.x()) * Scale(scale);
    node->setTransform(transform);

    if (nodeData["components"] && nodeData["components"].IsSequence()) {
        for (auto compData : nodeData["components"]) {
            if (compData["type"]) {
                std::string type = compData["type"].as<std::string>();
                auto comp = ComponentFactory::getInstance().createComponent(type);
                if (comp) {
                    comp->load(compData, physicsEngine, inputHandler);
                    node->addComponent(comp);
                } else {
                    Logger::Warn("Unknown component type: ", type);
                }
            }
        }
    }

    if (nodeData["children"] && nodeData["children"].IsSequence()) {
        for (auto childData : nodeData["children"]) {
            auto childNode = parseNode(childData, physicsEngine, inputHandler);
            if (childNode) {
                node->addChild(childNode);
            }
        }
    }

    return node;
}
