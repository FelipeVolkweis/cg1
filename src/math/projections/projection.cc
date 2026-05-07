#include "projection.h"

#include "utils/deg2rad.h"
#include "utils/logger.h"

/*
https://www.songho.ca/opengl/gl_projectionmatrix.html
    tangent = tan(fovy / 2)
    n = zNear
    f = zFar
    t = n * tangent
    r = t * aspect
    M = (n/r 0         0              0       )
        (0   n/t       0              0       )
        (0   0 -(f + n)/(f - n) (-2fn)/(f - n))
        (0   0        -1              0       )
*/
Mat4x4 Projection::getPerspective(float fovy, float aspect, float zNear, float zFar) {
    Mat4x4 mat;
    float fovRad = fovy * DEG2RAD;
    float n = zNear;
    float f = zFar;
    float t = n * std::tan(fovRad / 2);
    float r = t * aspect;

    mat << n/r, 0,         0,                     0,
            0, n/t,        0,                     0,
            0,  0, -(f + n)/(f - n), (-2 * f * n) / (f - n),
            0,  0,        -1,                     0;
    
    return mat;
}

Mat4x4 Projection::getOrthographic(float left, float right, float bottom, float top, float zNear,
                                   float zFar) {
    float l = left;
    float r = right;
    float b = bottom;
    float t = top;
    float n = zNear;
    float f = zFar;

    Mat4x4 mat;
    mat << 2 / (r - l), 0, 0,                    -(r + l) / (r - l),
             0,       2 / (t - b),   0,          -(t + b) / (t - b),
             0,         0,        -2 / (f - n),  -(f + n) / (f - n),
             0,         0,           0,                   1;

    return mat;
}

Mat4x4 Projection::getLookAt(Vec3 position, Vec3 focalPoint, Vec3 up) {
    Mat4x4 mat;
    Vec3 zc = position - focalPoint;
    if (zc.norm() < 1e-6) {
        Logger::Warn("zc's norm is 0");
        return mat;
    }

    zc.normalize();

    Vec3 xc = up.cross(zc);
    if (xc.norm() < 1e-6) {
        Logger::Warn("xc's norm is 0");
        return mat;
    }

    xc.normalize();

    Vec3 yc = zc.cross(xc);

    Mat4x4 r;
    r << xc.transpose(), 0,
         yc.transpose(), 0,
         zc.transpose(), 0,
         0, 0, 0, 1;
    
    Mat4x4 t;
    t << 1, 0, 0, -position.x(),
         0, 1, 0, -position.y(),
         0, 0, 1, -position.z(),
         0, 0, 0, 1;

    return r * t;
}