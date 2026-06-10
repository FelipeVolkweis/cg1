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

    void load(const YAML::Node &data) override;

    std::shared_ptr<RenderableLight> getRenderableLight() const {
        return renderableLight_;
    }

    Kind getKind() const {
        return kind_;
    }

    void setAmbientOffset(const Vec3 &v) {
        ambientOffset_ = v;
    }

    void setDiffuseOffset(const Vec3 &v) {
        diffuseOffset_ = v;
    }

    void setSpecularOffset(const Vec3 &v) {
        specularOffset_ = v;
    }

    void setEnabled(bool e) {
        enabled_ = e;
    }

    bool isEnabled() const {
        return enabled_;
    }

    const Vec3 &getAmbient() const {
        return ambient_;
    }

    const Vec3 &getDiffuse() const {
        return diffuse_;
    }

    const Vec3 &getSpecular() const {
        return specular_;
    }

    void onUpdate(float dt) override;

private:
    std::shared_ptr<BaseLight> light_;
    std::shared_ptr<RenderableLight> renderableLight_;
    Kind kind_;

    Vec3 ambient_;
    Vec3 diffuse_;
    Vec3 specular_;
    Vec3 ambientOffset_ = Vec3::Zero();
    Vec3 diffuseOffset_ = Vec3::Zero();
    Vec3 specularOffset_ = Vec3::Zero();
    Vec3 direction_ = Vec3(0, 0, -1);
    float fadeDistance_ = 10.0f;
    float cutoff_ = 15.0f;
    float outerCutoff_ = 20.0f;
    bool enabled_ = true;
};

#endif
