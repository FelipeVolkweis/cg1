#ifndef CAMERA_H
#define CAMERA_H

#include "types/mat4x4.h"
#include "types/vec3.h"

class Camera {
public:
    Camera(float fovy, float aspect, float zNear, float zFar);

    Mat4x4 perspective();

private:
    float fovy_;
    float aspect_;
    float zNear_;
    float zFar_;
};

#endif