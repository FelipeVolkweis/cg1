#include "baseshape.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include "renderer/texture.h"
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
            if (!isModel())
                meshGroups_->push_back(
                    {Texture::createColorTexture(color_), 0, vertices_->size(), false});
        }
    }
}