#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <memory>
#include <string>

#include "core/component.h"
#include "renderer/lights/renderablelight.h"
#include "types/vec3.h"

/**
 * @class LightComponent
 * @brief Component that represents a light source in the scene.
 */
class LightComponent : public BaseComponent {
public:
    enum class Kind { Directional, Point, Spot };

    LightComponent() = default;

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    std::shared_ptr<RenderableLight> getRenderableLight() const {
        return renderableLight_;
    }

    Kind getKind() const {
        return kind_;
    }

    void onUpdate(float dt) override;

private:
    std::shared_ptr<BaseLight> light_;
    std::shared_ptr<RenderableLight> renderableLight_;
    Kind kind_;

    Vec3 ambient_;
    Vec3 diffuse_;
    Vec3 specular_;
    Vec3 direction_ = Vec3(0, 0, -1);
    float fadeDistance_ = 10.0f;
    float cutoff_ = 15.0f;
    float outerCutoff_ = 20.0f;
};

#endif
