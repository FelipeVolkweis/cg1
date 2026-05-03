#include "meshcomponent.h"

#include <yaml-cpp/yaml.h>

#include "shapes/box.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/semisphere.h"
#include "shapes/sphere.h"
#include "shapes/torus.h"

void MeshComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                         InputHandler &inputHandler) {
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
        }

        if (shape_) {
            shape_->parse(data);
        }
    }
}
