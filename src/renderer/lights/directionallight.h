#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "baselight.h"

class DirectionalLight : public BaseLight {
public:
    DirectionalLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 direction);

private:
    Vec3 direction_;
};

#endif