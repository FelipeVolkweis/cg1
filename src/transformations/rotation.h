#ifndef ROTATION_H
#define ROTATION_H

#include "transformation.h"

#define DEG2RAD (M_PI / 180.0f)

class RotationX : public Transformation {
public:
    RotationX(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cosf(angInRad);
        float sin = std::sinf(angInRad);

        matrix_ << 
            1, 0, 0, 0,
            0, cos, -sin, 0,
            0, sin,  cos, 0,
            0, 0, 0, 1;
    }
};

class RotationY : public Transformation {
public:
    RotationY(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cosf(angInRad);
        float sin = std::sinf(angInRad);

        matrix_ << 
            cos,  0, sin, 0,
            0, 1, 0, 0,
            -sin, 0, cos, 0,
            0, 0, 0, 1;
    }
};

class RotationZ : public Transformation {
public: 
    RotationZ(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cosf(angInRad);
        float sin = std::sinf(angInRad);

        matrix_ << 
            cos, -sin, 0, 0,
            sin, cos, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
    }
};

#endif