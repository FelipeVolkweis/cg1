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

class Camera {
public:
    Camera(Perspective perspective, LookAt lookAt);

    Mat4x4 lookAt();
    Mat4x4 perspective();

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