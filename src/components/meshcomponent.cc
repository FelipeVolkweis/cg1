#include "meshcomponent.h"

#include <yaml-cpp/yaml.h>

#include "shapes/box.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/modelshape.h"
#include "shapes/semisphere.h"
#include "shapes/sphere.h"
#include "shapes/torus.h"

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

            if (data["materials"] && data["materials"].IsSequence()) {
                std::vector<Material> overrideMaterials;
                for (const auto &matNode : data["materials"]) {
                    overrideMaterials.push_back(Material::fromYaml(matNode.as<std::string>()));
                }

                if (!overrideMaterials.empty()) {
                    auto meshGroups = shape_->getMeshGroups();
                    for (size_t i = 0; i < meshGroups->size(); ++i) {
                        size_t matIdx = std::min(i, overrideMaterials.size() - 1);
                        (*meshGroups)[i].material = overrideMaterials[matIdx];
                        // If it's translucent, update the translucent flag
                        (*meshGroups)[i].translucent = overrideMaterials[matIdx].getDissolve() < 1.0f;
                    }
                }
            }
        }

        renderableMesh_ = shape_->asRenderable();
    }
}
