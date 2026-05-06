#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "baselight.h"

class PointLight : public BaseLight {
public:
    PointLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, float fadeDistance);

private:
    Vec3 position_;

    float fadeDistance_;

    float constant_;
    float linear_;
    float quadratic_;
};

#endif