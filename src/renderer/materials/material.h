#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "types/vec3.h"

class Material {
public:
    Material(uint32_t diffuseMap, uint32_t specularMap, const Vec3 &ambientColor,
             const Vec3 &diffuseColor, const Vec3 &specularColor, float shininess,
             float dissolve = 1.0f);

    static Material fromYaml(const std::string &name);
    static Material overlayYaml(const Material &base, const std::string &name);

    uint32_t getDiffuseMap() const {
        return diffuseMap_;
    }

    uint32_t getSpecularMap() const {
        return specularMap_;
    }

    const Vec3 &getAmbientColor() const {
        return ambientColor_;
    }

    const Vec3 &getDiffuseColor() const {
        return diffuseColor_;
    }

    const Vec3 &getSpecularColor() const {
        return specularColor_;
    }

    float getShininess() const {
        return shininess_;
    }

    float getDissolve() const {
        return dissolve_;
    }

private:
    uint32_t diffuseMap_;
    uint32_t specularMap_;

    Vec3 ambientColor_;
    Vec3 diffuseColor_;
    Vec3 specularColor_;

    float shininess_;
    float dissolve_;
};

#endif