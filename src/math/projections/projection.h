#ifndef PROJECTION_H
#define PROJECTION_H

#include "types/mat4x4.h"
#include "types/vec3.h"

class Projection {
public:
    static Mat4x4 getPerspective(float fovy, float aspect, float zNear, float zFar);
    static Mat4x4 getOrthographic(float left, float right, float bottom, float top, float zNear,
                                  float zFar);
    static Mat4x4 getLookAt(Vec3 position, Vec3 focalPoint, Vec3 up);

    static Mat4x4 getLightSpaceMatrix(const Mat4x4 &cameraProjection, const Mat4x4 &cameraView,
                                      const Vec3 &lightDirection);
};

#endif