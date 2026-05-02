#include "camera.h"

#include "utils/deg2rad.h"

Camera::Camera(float fovy, float aspect, float zNear, float zFar) : 
    fovy_(fovy), aspect_(aspect), zNear_(zNear), zFar_(zFar) {}

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
