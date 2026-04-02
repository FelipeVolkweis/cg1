#ifndef SCENE_H
#define SCENE_H

#include "renderer/renderer.h"
#include "shapes/baseshape.h"
#include "transformations/transformation.h"
#include <memory>
#include <string>
#include <vector>

class Scene {
public:
    struct Subscene {
        std::unique_ptr<Scene> scene;
        Transformation transform;
    };

    Scene() = default;

    bool load(const std::string &filepath);
    void populateRenderer(Renderer &renderer,
                          const Transformation &parentTransform = Transformation());

private:
    std::vector<std::unique_ptr<BaseShape>> shapes_;
    std::vector<Subscene> subscenes_;
};

#endif