#ifndef ROTATION_H
#define ROTATION_H

#include <cmath>

#include "transformation.h"
#include "utils/deg2rad.h"
#include "types/quaternion.h"
#include "types/mat3x3.h"


/**
 * @class RotationX
 * @brief Represents a rotation transformation around the X-axis.
 */
class RotationX : public Transformation {
public:
    RotationX(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cos(angInRad);
        float sin = std::sin(angInRad);

        matrix_ << 
            1, 0, 0, 0,
            0, cos, -sin, 0,
            0, sin,  cos, 0,
            0, 0, 0, 1;
    }
};

/**
 * @class RotationY
 * @brief Represents a rotation transformation around the Y-axis.
 */
class RotationY : public Transformation {
public:
    RotationY(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cos(angInRad);
        float sin = std::sin(angInRad);

        matrix_ << 
            cos,  0, sin, 0,
            0, 1, 0, 0,
            -sin, 0, cos, 0,
            0, 0, 0, 1;
    }
};

/**
 * @class RotationZ
 * @brief Represents a rotation transformation around the Z-axis.
 */
class RotationZ : public Transformation {
public:
    RotationZ(float angleInDegrees) {
        float angInRad = DEG2RAD * angleInDegrees;
        float cos = std::cos(angInRad);
        float sin = std::sin(angInRad);

        matrix_ << 
            cos, -sin, 0, 0,
            sin, cos, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
    }
};

class QuaternionRotation : public Transformation {
public:
    QuaternionRotation() = default;
    QuaternionRotation(float xInDegrees, float yInDegrees, float zInDegrees) {
        float x = DEG2RAD * xInDegrees;
        float y = DEG2RAD * yInDegrees;
        float z = DEG2RAD * zInDegrees;
        quat_ = Eigen::AngleAxisf(z, Eigen::Vector3f::UnitZ()) *
            Eigen::AngleAxisf(y, Eigen::Vector3f::UnitY()) *
            Eigen::AngleAxisf(x, Eigen::Vector3f::UnitX());

        Mat3x3 rot3x3 = quat_.toRotationMatrix();

        matrix_ = Mat4x4::Identity();
        matrix_.block<3, 3>(0, 0) = rot3x3;
    }

    Quaternion &getQuaternion() {
        return quat_;
    }
private:
    Quaternion quat_;
};

#endif