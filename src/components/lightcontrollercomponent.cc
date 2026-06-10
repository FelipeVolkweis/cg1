#include "lightcontrollercomponent.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

#include "components/lightcomponent.h"
#include "core/node.h"
#include "window/inputhandler.h"
#include "window/window.h"

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

    int ambDelta = inputHandler.getLightAmbientDelta();
    if (ambDelta != 0)
        ambientOffset_ += Vec3(1, 1, 1) * (ambDelta * delta);

    int difDelta = inputHandler.getLightDiffuseDelta();
    if (difDelta != 0)
        diffuseOffset_ += Vec3(1, 1, 1) * (difDelta * delta);

    int spcDelta = inputHandler.getLightSpecularDelta();
    if (spcDelta != 0)
        specularOffset_ += Vec3(1, 1, 1) * (spcDelta * delta);

    lightComp->setAmbientOffset(ambientOffset_);
    lightComp->setDiffuseOffset(diffuseOffset_);
    lightComp->setSpecularOffset(specularOffset_);
}
