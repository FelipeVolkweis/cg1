#include "spotlight.h"

Spotlight::Spotlight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 direction,
                     float cutoff, float outerCutoff, float fadeDistance)
    : BaseLight(ambient, diffuse, specular), position_(position), direction_(direction),
      cutoff_(cutoff), outerCutoff_(outerCutoff) {
    for (const auto &entry : Light::table) {
        if (fadeDistance <= entry.distance) {
            constant_ = entry.constant;
            linear_ = entry.linear;
            quadratic_ = entry.quadratic;
        }
    }
}
