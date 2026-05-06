#ifndef MATERIAL_H
#define MATERIAL_H

#include "types/vec3.h"

class Material {
public:
    Material(uint32_t diffuseTextureId, uint32_t specularTextureId, float shininess,
             float dissolve = 1.0f);

    uint32_t getDiffuse() const {
        return diffuseTextureId_;
    }

    uint32_t getSpecular() const {
        return specularTextureId_;
    }

    float getShininess() const {
        return shininess_;
    }

    float getDissolve() const {
        return dissolve_;
    }

private:
    uint32_t diffuseTextureId_;
    uint32_t specularTextureId_;

    float shininess_;
    float dissolve_;
};

#endif