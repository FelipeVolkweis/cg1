#include "scene.h"
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
            parseSubscene(iss);
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

void Scene::parseSubscene(std::istringstream &iss) {
    std::string subFilepath;
    if (iss >> subFilepath) {
        auto subscene = std::make_unique<Scene>();
        subFilepath = "data/subscenes/" + subFilepath + ".txt";
        if (subscene->load(subFilepath)) {
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

            subscenes_.push_back({std::move(subscene), finalTransform, modifiers});
        } else {
            Logger::Warn("Failed to load subscene from ", subFilepath);
        }
    }
}

void Scene::populateRenderer(
    Renderer &renderer, const std::unordered_map<std::string, Transformation> &modifierTransforms,
    const Transformation &parentTransform) {
    for (const auto &shape : shapes_) {
        Transformation world = parentTransform * shape->getTransformation();
        applyDynamicModifiers(world, shape->getModifiers(), modifierTransforms);

        renderer.addShape(shape.get(), world);
    }

    for (const auto &sub : subscenes_) {
        Transformation world = parentTransform * sub.transform;
        applyDynamicModifiers(world, sub.modifiers, modifierTransforms);

        sub.scene->populateRenderer(renderer, modifierTransforms, world);
    }
}

void Scene::applyDynamicModifiers(
    Transformation &world, const std::vector<std::string> &modifiers,
    const std::unordered_map<std::string, Transformation> &modifierTransforms) const {
    Transformation dynamic;
    bool hasDynamic = false;
    for (const auto &m : modifiers) {
        auto it = modifierTransforms.find(m);
        if (it != modifierTransforms.end()) {
            dynamic = it->second * dynamic;
            hasDynamic = true;
        }
    }

    if (hasDynamic) {
        Vec3 origin(world.getTransformationMatrix()(0, 3), world.getTransformationMatrix()(1, 3),
                    world.getTransformationMatrix()(2, 3));
        Transformation toOrigin = Translation(Vec3(-origin.x(), -origin.y(), -origin.z()));
        Transformation backToPos = Translation(origin);
        world = backToPos * dynamic * toOrigin * world;
    }
}