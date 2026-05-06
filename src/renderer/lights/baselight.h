#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "types/vec3.h"

class BaseLight {
public:
    BaseLight(Vec3 ambient, Vec3 diffuse, Vec3 specular);

protected:
    Vec3 ambient_;
    Vec3 diffuse_;
    Vec3 specular_;
};

#endif