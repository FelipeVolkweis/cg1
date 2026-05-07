#include "lightcomponent.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/node.h"
#include "renderer/lights/directionallight.h"
#include "renderer/lights/pointlight.h"
#include "renderer/lights/spotlight.h"

void LightComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                          InputHandler &inputHandler) {
    std::string kindStr = data["kind"].as<std::string>();

    auto node = getNode();

    ambient_ = Light::defaultAmbient;
    if (data["ambient"])
        ambient_ = Vec3(data["ambient"][0].as<float>(), data["ambient"][1].as<float>(),
                        data["ambient"][2].as<float>());

    diffuse_ = Light::defaultDiffuse;
    if (data["diffuse"])
        diffuse_ = Vec3(data["diffuse"][0].as<float>(), data["diffuse"][1].as<float>(),
                        data["diffuse"][2].as<float>());

    specular_ = Light::defaultSpecular;
    if (data["specular"])
        specular_ = Vec3(data["specular"][0].as<float>(), data["specular"][1].as<float>(),
                         data["specular"][2].as<float>());

    direction_ = Vec3(0, 0, -1);
    if (data["direction"])
        direction_ = Vec3(data["direction"][0].as<float>(), data["direction"][1].as<float>(),
                          data["direction"][2].as<float>());

    if (kindStr == "directional") {
        kind_ = Kind::Directional;
        renderableLight_ = std::make_shared<RenderableDirectionalLight>(node->getId());
        light_ = std::make_shared<DirectionalLight>(ambient_, diffuse_, specular_, direction_);
    } else if (kindStr == "point") {
        kind_ = Kind::Point;
        if (data["fadeDistance"])
            fadeDistance_ = data["fadeDistance"].as<float>();

        renderableLight_ = std::make_shared<RenderablePointLight>(node->getId());
        light_ = std::make_shared<PointLight>(ambient_, diffuse_, specular_, Vec3::Zero(),
                                              fadeDistance_);
    } else if (kindStr == "spot") {
        kind_ = Kind::Spot;
        if (data["cutoff"])
            cutoff_ = data["cutoff"].as<float>();
        if (data["outerCutoff"])
            outerCutoff_ = data["outerCutoff"].as<float>();
        if (data["fadeDistance"])
            fadeDistance_ = data["fadeDistance"].as<float>();

        renderableLight_ = std::make_shared<RenderableSpotlight>(node->getId());
        light_ =
            std::make_shared<Spotlight>(ambient_, diffuse_, specular_, Vec3::Zero(), direction_,
                                        cutoff_ * DEG2RAD, outerCutoff_ * DEG2RAD, fadeDistance_);
    }

    if (renderableLight_ && light_) {
        renderableLight_->setLight(light_);
    }
}

void LightComponent::onUpdate(float dt) {
    auto node = getNode();
    if (!node)
        return;

    Transformation globalTransform;
    auto current = node;
    while (current != nullptr) {
        globalTransform = current->getTransformation() * globalTransform;
        current = current->getParent();
    }

    Mat4x4 mat = globalTransform.getTransformationMatrix();
    Vec3 position = mat.block<3, 1>(0, 3);
    Vec3 worldDirection = (mat.block<3, 3>(0, 0) * direction_).normalized();

    if (kind_ == Kind::Directional) {
        light_ = std::make_shared<DirectionalLight>(ambient_, diffuse_, specular_, worldDirection);
    } else if (kind_ == Kind::Point) {
        light_ =
            std::make_shared<PointLight>(ambient_, diffuse_, specular_, position, fadeDistance_);
    } else if (kind_ == Kind::Spot) {
        light_ =
            std::make_shared<Spotlight>(ambient_, diffuse_, specular_, position, worldDirection,
                                        cutoff_ * DEG2RAD, outerCutoff_ * DEG2RAD, fadeDistance_);
    }

    if (renderableLight_)
        renderableLight_->setLight(light_);
}
