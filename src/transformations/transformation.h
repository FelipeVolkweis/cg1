#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "types/mat4x4.h"

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
};

#endif
