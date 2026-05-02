#include "camera.h"

#include "utils/deg2rad.h"
#include "utils/logger.h"

Camera::Camera(Perspective perspective, LookAt lookAt) : 
    fovy_(perspective.fovy), aspect_(perspective.aspect), 
    zNear_(perspective.zNear), zFar_(perspective.zFar),
    position_(lookAt.position), focalPoint_(lookAt.focalPoint), 
    up_(lookAt.up) {}

Mat4x4 Camera::lookAt() {
    Mat4x4 mat;
    Vec3 zc = position_ - focalPoint_;
    if (zc.norm() < 1e-6) {
        Logger::Warn("zc's norm is 0");
        return mat;
    }

    zc.normalize();

    Vec3 xc = up_.cross(zc);
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
    t << 1, 0, 0, -position_.x(),
         0, 1, 0, -position_.y(),
         0, 0, 1, -position_.z(),
         0, 0, 0, 1;

    return r * t;
}

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
Mat4x4 Camera::perspective() {
    Mat4x4 mat;
    float fovRad = fovy_ * DEG2RAD;
    float n = zNear_;
    float f = zFar_;
    float t = n * std::tanf(fovRad / 2);
    float r = t * aspect_;

    mat << n/r, 0,         0,                     0,
            0, n/t,        0,                     0,
            0,  0, -(f + n)/(f - n), (-2 * f * n) / (f - n),
            0,  0,        -1,                     0;
    
    return mat;
}
