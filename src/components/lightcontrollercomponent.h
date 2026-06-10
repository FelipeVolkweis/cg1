#ifndef LIGHT_CONTROLLER_COMPONENT_H
#define LIGHT_CONTROLLER_COMPONENT_H

#include "core/component.h"
#include "types/vec3.h"

/**
 * @class LightControllerComponent
 * @brief Component that provides keyboard controls for adjusting a sibling LightComponent.
 *
 * When an optional index (0-9) is assigned, the light can be selected via the number keys.
 * Controls:
 *   U     - Toggle light on/off
 *   R / F - Increase / decrease ambient
 *   T / G - Increase / decrease diffuse
 *   Y / H - Increase / decrease specular
 */
class LightControllerComponent : public BaseComponent {
public:
    LightControllerComponent() = default;

    void load(const YAML::Node &data) override;
    void onUpdate(float dt) override;

private:
    int lightIndex_ = -1;
    float adjustSpeed_ = 0.5f;

    Vec3 ambientOffset_ = Vec3::Zero();
    Vec3 diffuseOffset_ = Vec3::Zero();
    Vec3 specularOffset_ = Vec3::Zero();
};

#endif
