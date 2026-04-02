#include "baseshape.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"
#include <string>

void BaseShape::parseCommon(std::istream &is) {
    std::string key;

    Vec3 origin(0.0f, 0.0f, 0.0f);
    Vec3 scale(1.0f, 1.0f, 1.0f);
    Vec3 rot(0.0f, 0.0f, 0.0f);

    while (is >> key) {
        if (key == "color") {
            float r, g, b;
            if (is >> r >> g >> b) {
                color_ = Vec3(r, g, b);
            }
        } else if (key == "origin") {
            float x, y, z;
            if (is >> x >> y >> z) {
                origin = Vec3(x, y, z);
            }
        } else if (key == "scale") {
            float x, y, z;
            if (is >> x >> y >> z) {
                scale = Vec3(x, y, z);
            }
        } else if (key == "rot") {
            float x, y, z;
            if (is >> x >> y >> z) {
                rot = Vec3(x, y, z);
            }
        } else {
            Logger::Warn("Unknown common property key: ", key);
        }
    }

    Transformation finalTransform = Translation(origin) * RotationZ(rot.z()) * RotationY(rot.y()) *
                                    RotationX(rot.x()) * Scale(scale);

    setTransformation(finalTransform);
}