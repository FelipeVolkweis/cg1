#ifndef SCENE_H
#define SCENE_H

#include "renderer/renderer.h"
#include "shapes/baseshape.h"
#include "transformations/transformation.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Scene {
public:
    struct Subscene {
        std::unique_ptr<Scene> scene;
        Transformation transform;
        std::vector<std::string> modifiers;
    };

    Scene() = default;

    bool load(const std::string &filepath);
    void
    populateRenderer(Renderer &renderer,
                     const std::unordered_map<std::string, Transformation> &modifierTransforms = {},
                     const Transformation &parentTransform = Transformation());

private:
    std::unique_ptr<BaseShape> makeShape(const std::string &name);
    void parseSubscene(std::istringstream &iss);

    void applyDynamicModifiers(
        Transformation &world, const std::vector<std::string> &modifiers,
        const std::unordered_map<std::string, Transformation> &modifierTransforms) const;

    std::vector<std::unique_ptr<BaseShape>> shapes_;
    std::vector<Subscene> subscenes_;
};

#endif