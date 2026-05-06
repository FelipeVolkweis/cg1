#include "pointlight.h"

PointLight::PointLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, float fadeDistance)
    : BaseLight(ambient, diffuse, specular), position_(position), fadeDistance_(fadeDistance) {
    for (const auto &entry : Light::table) {
        if (fadeDistance <= entry.distance) {
            constant_ = entry.constant;
            linear_ = entry.linear;
            quadratic_ = entry.quadratic;
        }
    }
}