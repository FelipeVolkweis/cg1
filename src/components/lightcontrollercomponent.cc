#include "lightcontrollercomponent.h"

#include <algorithm>

#include <yaml-cpp/yaml.h>

#include "components/lightcomponent.h"
#include "core/node.h"
#include "window/inputhandler.h"
#include "window/window.h"

namespace {
Vec3 clampColor(const Vec3 &color) {
    return Vec3(std::clamp(color.x(), 0.0f, 1.0f), std::clamp(color.y(), 0.0f, 1.0f),
                std::clamp(color.z(), 0.0f, 1.0f));
}

void adjustOffset(Vec3 &offset, const Vec3 &baseColor, int direction, float delta) {
    if (direction == 0)
        return;

    Vec3 color = baseColor + offset + Vec3::Ones() * (direction * delta);
    offset = clampColor(color) - baseColor;
}
} // namespace

void LightControllerComponent::load(const YAML::Node &data) {
    if (data["index"])
        lightIndex_ = data["index"].as<int>();
    if (data["adjustSpeed"])
        adjustSpeed_ = data["adjustSpeed"].as<float>();
}

void LightControllerComponent::onUpdate(float dt) {
    auto &inputHandler = Window::instance().getInputHandler();

    if (lightIndex_ >= 0 && inputHandler.getSelectedLightIndex() != lightIndex_)
        return;

    auto node = getNode();
    if (!node)
        return;

    auto lightComp = node->getComponent<LightComponent>();
    if (!lightComp)
        return;

    if (inputHandler.consumeLightToggleRequest()) {
        bool newState = !lightComp->isEnabled();
        lightComp->setEnabled(newState);
    }

    if (!lightComp->isEnabled())
        return;

    float delta = adjustSpeed_ * dt;

    adjustOffset(ambientOffset_, lightComp->getAmbient(), inputHandler.getLightAmbientDelta(),
                 delta);
    adjustOffset(diffuseOffset_, lightComp->getDiffuse(), inputHandler.getLightDiffuseDelta(),
                 delta);
    adjustOffset(specularOffset_, lightComp->getSpecular(), inputHandler.getLightSpecularDelta(),
                 delta);

    lightComp->setAmbientOffset(ambientOffset_);
    lightComp->setDiffuseOffset(diffuseOffset_);
    lightComp->setSpecularOffset(specularOffset_);
}
