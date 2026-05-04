#ifndef CAMERA_H
#define CAMERA_H

#include "types/mat4x4.h"
#include "types/vec3.h"

struct Perspective {
    float fovy, aspect, zNear, zFar;
};

struct LookAt {
    Vec3 position;
    Vec3 focalPoint;
    Vec3 up;
};

/**
 * @brief Class representing a camera in the 3D scene.
 */
class Camera {
public:
    /**
     * @brief Constructs a new Camera object.
     *
     * @param perspective The perspective projection parameters.
     * @param lookAt The look-at parameters.
     */
    Camera(Perspective perspective, LookAt lookAt);

    /**
     * @brief Calculates the look-at matrix.
     *
     * @return Mat4x4 The look-at matrix.
     */
    Mat4x4 lookAt();

    /**
     * @brief Calculates the perspective projection matrix.
     *
     * @return Mat4x4 The perspective projection matrix.
     */
    Mat4x4 perspective();

    void setPosition(const Vec3 &position) {
        position_ = position;
    }

    void setFocalPoint(const Vec3 &focalPoint) {
        focalPoint_ = focalPoint;
    }

    void setUp(const Vec3 &up) {
        up_ = up;
    }

    const Vec3 &getPosition() const {
        return position_;
    }

    const Vec3 &getFocalPoint() const {
        return focalPoint_;
    }

    const Vec3 &getUp() const {
        return up_;
    }

private:
    float fovy_;
    float aspect_;
    float zNear_;
    float zFar_;

    Vec3 position_;
    Vec3 focalPoint_;
    Vec3 up_;
};

#endif
