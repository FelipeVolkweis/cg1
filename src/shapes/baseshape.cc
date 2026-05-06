#include "baseshape.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include "renderer/textures/texture.h"
#include "utils/logger.h"

void BaseShape::initializeCommon() {
    if (!isModel()) {
        vertices_ = std::make_shared<std::vector<Vertex>>();
        meshGroups_ = std::make_shared<std::vector<MeshGroup>>();
    }
}

void BaseShape::parseCommon(const YAML::Node &node) {
    if (node["color"]) {
        auto colorNode = node["color"];
        if (colorNode.IsSequence() && colorNode.size() == 3) {
            color_ =
                Vec3(colorNode[0].as<float>(), colorNode[1].as<float>(), colorNode[2].as<float>());
            hasColor_ = true;
            if (!isModel()) {
                Vec3 gray(0.5, 0.5, 0.5);
                float defaultShininess = 0.5;
                Material material(Texture::createColorTexture(color_),
                                  Texture::createColorTexture(gray), defaultShininess);
                meshGroups_->push_back({material, 0, vertices_->size(), false});
            }
        }
    }
}