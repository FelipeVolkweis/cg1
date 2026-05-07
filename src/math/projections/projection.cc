#include "projection.h"

#include "utils/deg2rad.h"
#include "utils/logger.h"

#include "types/vec4.h"

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

// https://learnopengl.com/Guest-Articles/2021/CSM
Mat4x4 Projection::getLightSpaceMatrix(const Mat4x4 &cameraProjection, const Mat4x4 &cameraView, const Vec3 &lightDirection) {
    Mat4x4 invCam = (cameraProjection * cameraView).inverse();

    std::vector<Vec4> frustumCorners = {
        // Near face
        {-1.0f,  1.0f, -1.0f, 1.0f}, { 1.0f,  1.0f, -1.0f, 1.0f},
        { 1.0f, -1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f, 1.0f},
        // Far face
        {-1.0f,  1.0f,  1.0f, 1.0f}, { 1.0f,  1.0f,  1.0f, 1.0f},
        { 1.0f, -1.0f,  1.0f, 1.0f}, {-1.0f, -1.0f,  1.0f, 1.0f}
    };

    for (auto& corner : frustumCorners) {
        Vec4 pt = invCam * corner;
        corner = pt / pt.w();
    }

    Vec3 center = Vec3::Zero();

    for (const auto& v : frustumCorners) {
        center += Vec3(v.x(), v.y(), v.z());
    }
    center /= frustumCorners.size();

    Mat4x4 lightView = Projection::getLookAt(center - lightDirection, center, Vec3(0.0f, 1.0f, 0.0f));
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& corner : frustumCorners) {
        Vec4 trf = lightView * corner;
        
        minX = std::min(minX, trf.x());
        maxX = std::max(maxX, trf.x());
        minY = std::min(minY, trf.y());
        maxY = std::max(maxY, trf.y());
        minZ = std::min(minZ, trf.z());
        maxZ = std::max(maxZ, trf.z());
    }

    constexpr float zMult = 10.0f;
    if (minZ < 0) {
        minZ *= zMult;
    } else {
        minZ /= zMult;
    }
    if (maxZ < 0) {
        maxZ /= zMult;
    } else {
        maxZ *= zMult;
    }

    Mat4x4 lightProjection = Projection::getOrthographic(minX, maxX, minY, maxY, minZ, maxZ);
    Mat4x4 lightSpaceMatrix = lightProjection * lightView;

    return lightSpaceMatrix;
}
