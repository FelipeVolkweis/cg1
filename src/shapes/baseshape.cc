#include "baseshape.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include "utils/logger.h"

void BaseShape::parseCommon(const YAML::Node &node) {
    if (node["color"]) {
        auto colorNode = node["color"];
        if (colorNode.IsSequence() && colorNode.size() == 3) {
            color_ =
                Vec3(colorNode[0].as<float>(), colorNode[1].as<float>(), colorNode[2].as<float>());
        }
    }
}