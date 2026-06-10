#include "lightcomponent.h"

#include <algorithm>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "core/node.h"
#include "renderer/lights/directionallight.h"
#include "renderer/lights/pointlight.h"
#include "renderer/lights/spotlight.h"

void LightComponent::load(const YAML::Node &data) {
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

    Vec3 amb, dif, spc;
    if (enabled_) {
        auto clamp01 = [](const Vec3 &v) {
            return Vec3(std::clamp(v.x(), 0.0f, 1.0f), std::clamp(v.y(), 0.0f, 1.0f),
                        std::clamp(v.z(), 0.0f, 1.0f));
        };
        amb = clamp01(ambient_ + ambientOffset_);
        dif = clamp01(diffuse_ + diffuseOffset_);
        spc = clamp01(specular_ + specularOffset_);
    } else {
        amb = dif = spc = Vec3::Zero();
    }

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
        auto dl = std::static_pointer_cast<DirectionalLight>(light_);
        dl->setAmbient(amb);
        dl->setDiffuse(dif);
        dl->setSpecular(spc);
        dl->setDirection(worldDirection);
    } else if (kind_ == Kind::Point) {
        auto pl = std::static_pointer_cast<PointLight>(light_);
        pl->setAmbient(amb);
        pl->setDiffuse(dif);
        pl->setSpecular(spc);
        pl->setPosition(position);
    } else if (kind_ == Kind::Spot) {
        auto sl = std::static_pointer_cast<Spotlight>(light_);
        sl->setAmbient(amb);
        sl->setDiffuse(dif);
        sl->setSpecular(spc);
        sl->setPosition(position);
        sl->setDirection(worldDirection);
    }

    if (renderableLight_)
        renderableLight_->setLight(light_);
}
