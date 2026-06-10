#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "baselight.h"

class DirectionalLight : public BaseLight {
public:
    DirectionalLight() : BaseLight(), direction_(Light::defaultDirectional) {}

    DirectionalLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 direction)
        : BaseLight(ambient, diffuse, specular), direction_(direction) {}

    const Vec3 &getDirection() const {
        return direction_;
    }

    void setDirection(const Vec3 &v) {
        direction_ = v;
    }

private:
    Vec3 direction_;
};

#endif