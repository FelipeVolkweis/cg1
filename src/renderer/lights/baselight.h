#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "types/vec3.h"
#include "utils/deg2rad.h"

namespace Light {
static const Vec3 defaultAmbient(0.1f, 0.1f, 0.1f);
static const Vec3 defaultDiffuse(0.8f, 0.8f, 0.8f);
static const Vec3 defaultSpecular(1.0f, 1.0f, 1.0f);
static const Vec3 defaultDirectional(-0.2f, -1.0f, -0.3f);
static const Vec3 defaultPosition(0.0f, 3.0f, 0.0f);
static const float defaultFadeDistance = 6.0f;
static const float defaultCutoff = DEG2RAD * 15.0f;

struct TableEntry {
    int distance;
    float constant;
    float linear;
    float quadratic;
};

// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
static const TableEntry table[] = {
    {7, 1.0f, 0.7f, 1.8f},        {13, 1.0f, 0.35f, 0.44f},     {20, 1.0f, 0.22f, 0.20f},
    {32, 1.0f, 0.14f, 0.07f},     {50, 1.0f, 0.09f, 0.032f},    {65, 1.0f, 0.07f, 0.017f},
    {100, 1.0f, 0.045f, 0.0075f}, {160, 1.0f, 0.027f, 0.0028f}, {200, 1.0f, 0.022f, 0.0019f},
    {325, 1.0f, 0.014f, 0.0007f}, {600, 1.0f, 0.007f, 0.0002f}, {3250, 1.0f, 0.0014f, 0.000007f}};
} // namespace Light

class BaseLight {
public:
    BaseLight()
        : ambient_(Light::defaultAmbient), diffuse_(Light::defaultDiffuse),
          specular_(Light::defaultSpecular) {}

    BaseLight(Vec3 ambient, Vec3 diffuse, Vec3 specular)
        : ambient_(ambient), diffuse_(diffuse), specular_(specular) {};

    const Vec3 getAmbient() const {
        return ambient_;
    }

    const Vec3 getDiffuse() const {
        return diffuse_;
    }

    const Vec3 getSpecular() const {
        return specular_;
    }

protected:
    Vec3 ambient_;
    Vec3 diffuse_;
    Vec3 specular_;
};

#endif