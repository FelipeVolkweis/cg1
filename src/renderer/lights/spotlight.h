#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "baselight.h"

class Spotlight : public BaseLight {
public:
    Spotlight()
        : BaseLight(), position_(Light::defaultPosition), direction_(Light::defaultDirectional),
          cutoff_(Light::defaultCutoff), outerCutoff_(Light::defaultCutoff + 0.05f),
          constant_(1.0f), linear_(0.09f), quadratic_(0.032f) {}

    Spotlight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 direction,
              float cutoff, float outerCutoff, float fadeDistance);

    const Vec3 getPosition() const {
        return position_;
    }

    const Vec3 getDirection() const {
        return direction_;
    }

    float getCutoff() const {
        return cutoff_;
    }

    float getOuterCutoff() const {
        return outerCutoff_;
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
    Vec3 direction_;
    float cutoff_;
    float outerCutoff_;

    float constant_;
    float linear_;
    float quadratic_;
};

#endif