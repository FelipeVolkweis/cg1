#include "modelshape.h"

#include <yaml-cpp/yaml.h>

#include "model/modelimporter.h"
#include "utils/logger.h"

void ModelShape::parse(const YAML::Node &node) {
    parseCommon(node);
    if (node["path"]) {
        std::string path = node["path"].as<std::string>();
        ModelImporter::loadObjInto(path, this);
    } else {
        Logger::Error("ModelShape requires a 'path' property.");
    }
}