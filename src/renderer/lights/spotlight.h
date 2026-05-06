#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "baselight.h"

class Spotlight : public BaseLight {
public:
    Spotlight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 direction,
              float cutoff);

private:
    Vec3 position_;
    Vec3 direction_;
    float cutoff;
};

#endif