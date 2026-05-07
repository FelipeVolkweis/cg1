#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "types/mat4x4.h"
#include "types/vec3.h"

/**
 * @class Transformation
 * @brief Base class for 3D transformations, storing a 4x4 matrix.
 */
class Transformation {
protected:
    Mat4x4 matrix_;

public:
    Transformation() : matrix_(Mat4x4::Identity()) {}

    Transformation(const Mat4x4 &m) : matrix_(m) {}

    virtual ~Transformation() = default;

    const Mat4x4 &getTransformationMatrix() const {
        return matrix_;
    }

    Transformation operator*(const Transformation &rhs) const {
        return Transformation(matrix_ * rhs.getTransformationMatrix());
    }

    Transformation &operator*=(const Transformation &rhs) {
        matrix_ = matrix_ * rhs.getTransformationMatrix();
        return *this;
    }

    const Vec3 getPosition() const {
        return Vec3(matrix_(0, 3), matrix_(1, 3), matrix_(2, 3));
    }
};

#endif
