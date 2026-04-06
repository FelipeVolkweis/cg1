#include "scene.h"
#include "shapes/sphere.h"
#include "shapes/box.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/torus.h"
#include "shapes/semisphere.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"
#include <fstream>
#include <sstream>

bool Scene::load(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Error("Failed to open scene file: ", filepath);
        return false;
    }

    shapes_.clear();
    subscenes_.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        if (name == "subscene") {
            std::string subFilepath;
            if (iss >> subFilepath) {
                auto subscene = std::make_unique<Scene>();
                subFilepath = "data/subscenes/" + subFilepath + ".txt";
                if (subscene->load(subFilepath)) {
                    std::string key;
                    Vec3 origin(0.0f, 0.0f, 0.0f);
                    Vec3 scale(1.0f, 1.0f, 1.0f);
                    Vec3 rot(0.0f, 0.0f, 0.0f);

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
                        } else {
                            Logger::Warn("Unknown subscene property key: ", key);
                        }
                    }

                    Transformation finalTransform = Translation(origin) * RotationZ(rot.z()) *
                                                    RotationY(rot.y()) * RotationX(rot.x()) *
                                                    Scale(scale);

                    subscenes_.push_back({std::move(subscene), finalTransform});
                } else {
                    Logger::Warn("Failed to load subscene from ", subFilepath);
                }
            }
            continue;
        }

        auto newShape = makeShape(name);

        if (newShape) {
            newShape->parse(iss);
            shapes_.push_back(std::move(newShape));
        } else {
            Logger::Warn("Unknown shape/object name in scene file: ", name);
        }
    }

    return true;
}

std::unique_ptr<BaseShape> Scene::makeShape(const std::string &name) {
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

void Scene::populateRenderer(Renderer &renderer, const Transformation &parentTransform) {
    for (const auto &shape : shapes_) {
        renderer.addShape(shape.get(), parentTransform * shape->getTransformation());
    }

    for (const auto &sub : subscenes_) {
        sub.scene->populateRenderer(renderer, parentTransform * sub.transform);
    }
}