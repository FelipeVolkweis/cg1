#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "transformation.h"
#include "types/vec3.h"

class Translation : public Transformation {
public:
    Translation(const Vec3 &d) {
        matrix_ << 
            1, 0, 0, d.x(),
            0, 1, 0, d.y(),
            0, 0, 1, d.z(),
            0, 0, 0, 1;
    }
};

#endif