#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "baselight.h"

class PointLight : public BaseLight {
public:
    PointLight()
        : BaseLight(), position_(Light::defaultPosition), fadeDistance_(Light::defaultFadeDistance),
          constant_(1.0f), linear_(0.09f), quadratic_(0.032f) {}

    PointLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, float fadeDistance);

    const Vec3 getPosition() const {
        return position_;
    }

    float getFadeDistance() const {
        return fadeDistance_;
    }

    float getConstant() const {
        return constant_;
    }

    float getLinear() const {
        return linear_;
    }

    float getQuadratic() const {
        return quadratic_;
    }

private:
    Vec3 position_;

    float fadeDistance_;

    float constant_;
    float linear_;
    float quadratic_;
};

#endif