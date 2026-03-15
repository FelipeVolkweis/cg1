#ifndef SCALE_H
#define SCALE_H

#include "transformation.h"
#include "types/vec3.h"

class Scale : public Transformation {
public:
    Scale(const Vec3 &s) {
        matrix_ << 
            s.x(), 0, 0, 0,
            0, s.y(), 0, 0,
            0, 0, s.z(), 0,
            0, 0, 0, 1;

    }
};

#endif 