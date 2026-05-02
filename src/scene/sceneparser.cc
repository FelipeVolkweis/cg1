#include "sceneparser.h"

#include <fstream>
#include <sstream>

#include "components/meshcomponent.h"
#include "shapes/box.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/semisphere.h"
#include "shapes/sphere.h"
#include "shapes/torus.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"

bool SceneParser::load(const std::string &filepath, std::shared_ptr<Node> root) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Error("Failed to open scene file: ", filepath);
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        if (name == "subscene") {
            auto newNode = parseSubscene(iss);
            if (newNode) {
                root->addChild(newNode);
            }
            continue;
        }

        auto newShape = makeShape(name);
        auto node = std::make_shared<Node>();
        if (newShape) {
            auto transform = newShape->parse(iss);
            node->setTransform(transform);
            auto mesh = std::make_shared<MeshComponent>(std::move(newShape));
            node->addComponent(mesh);
            root->addChild(node);
        } else {
            Logger::Warn("Unknown shape/object name in scene file: ", name);
        }
    }

    return true;
}

std::unique_ptr<BaseShape> SceneParser::makeShape(const std::string &name) {
    std::unique_ptr<BaseShape> newShape;
    if (name == "sphere") {
        newShape = std::make_unique<Sphere>();
    } else if (name == "box") {
        newShape = std::make_unique<Box>();
    } else if (name == "cone") {
        newShape = std::make_unique<Cone>();
    } else if (name == "cylinder") {
        newShape = std::make_unique<Cylinder>();
    } else if (name == "torus") {
        newShape = std::make_unique<Torus>();
    } else if (name == "semisphere") {
        newShape = std::make_unique<SemiSphere>();
    }

    return newShape;
}

std::shared_ptr<Node> SceneParser::parseSubscene(std::istringstream &iss) {
    std::string subFilepath;
    if (iss >> subFilepath) {
        auto node = std::make_shared<Node>();

        subFilepath = "data/subscenes/" + subFilepath + ".txt";
        if (load(subFilepath, node)) {
            std::string key;
            Vec3 origin(0.0f, 0.0f, 0.0f);
            Vec3 scale(1.0f, 1.0f, 1.0f);
            Vec3 rot(0.0f, 0.0f, 0.0f);
            std::vector<std::string> modifiers;

            while (iss >> key) {
                if (key == "origin") {
                    float x, y, z;
                    if (iss >> x >> y >> z)
                        origin = Vec3(x, y, z);
                } else if (key == "scale") {
                    float x, y, z;
                    if (iss >> x >> y >> z)
                        scale = Vec3(x, y, z);
                } else if (key == "rot") {
                    float x, y, z;
                    if (iss >> x >> y >> z)
                        rot = Vec3(x, y, z);
                } else if (key == "translatable" || key == "rotatable" || key == "scalable") {
                    modifiers.push_back(key);
                } else {
                    Logger::Warn("Unknown subscene property key: ", key);
                }
            }

            Transformation finalTransform = Translation(origin) * RotationZ(rot.z()) *
                                            RotationY(rot.y()) * RotationX(rot.x()) * Scale(scale);

            node->setTransform(finalTransform);

            return node;
        } else {
            Logger::Warn("Failed to load subscene from ", subFilepath);
        }
    }
    return nullptr;
}
