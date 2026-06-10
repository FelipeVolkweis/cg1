#include "meshcomponent.h"

#include <yaml-cpp/yaml.h>

#include "shapes/box.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/modelshape.h"
#include "shapes/semisphere.h"
#include "shapes/sphere.h"
#include "shapes/torus.h"
#include "utils/logger.h"

void MeshComponent::load(const YAML::Node &data) {
    if (data["shape"]) {
        std::string shapeType = data["shape"].as<std::string>();
        if (shapeType == "box") {
            shape_ = std::make_unique<Box>();
        } else if (shapeType == "sphere") {
            shape_ = std::make_unique<Sphere>();
        } else if (shapeType == "cone") {
            shape_ = std::make_unique<Cone>();
        } else if (shapeType == "cylinder") {
            shape_ = std::make_unique<Cylinder>();
        } else if (shapeType == "torus") {
            shape_ = std::make_unique<Torus>();
        } else if (shapeType == "semisphere") {
            shape_ = std::make_unique<SemiSphere>();
        } else if (shapeType == "model") {
            shape_ = std::make_unique<ModelShape>();
        }

        if (shape_) {
            shape_->parse(data);
            parseMaterials(data);
            renderableMesh_ = shape_->asRenderable();
        }
    }
}

void MeshComponent::parseMaterials(const YAML::Node &data) {
    if (data["materials"]) {
        auto meshGroups = shape_->getMeshGroups();

        if (data["materials"].IsSequence()) {
            std::vector<std::string> overrideNames;
            for (const auto &matNode : data["materials"]) {
                overrideNames.push_back(matNode.as<std::string>());
            }

            if (!overrideNames.empty()) {
                for (size_t i = 0; i < meshGroups->size() && i < overrideNames.size(); ++i) {
                    (*meshGroups)[i].material =
                        Material::overlayYaml((*meshGroups)[i].material, overrideNames[i]);
                    (*meshGroups)[i].translucent = (*meshGroups)[i].material.getDissolve() < 1.0f;
                }
            }
        } else if (data["materials"].IsMap()) {
            for (auto it = data["materials"].begin(); it != data["materials"].end(); ++it) {
                std::string targetName = it->first.as<std::string>();
                std::string matYaml = it->second.as<std::string>();

                bool found = false;
                for (auto &mg : *meshGroups) {
                    if (mg.name == targetName) {
                        mg.material = Material::overlayYaml(mg.material, matYaml);
                        mg.translucent = mg.material.getDissolve() < 1.0f;
                        found = true;
                    }
                }

                if (!found) {
                    Logger::Warn("Material override '", targetName, "' not found in shape.");
                    Logger::Warn("Available material names:");
                    for (auto &mg : *meshGroups) {
                        Logger::Warn("  - ", mg.name);
                    }
                }
            }
        }
    }
}