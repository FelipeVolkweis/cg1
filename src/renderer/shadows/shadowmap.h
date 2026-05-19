#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <cstdint>
#include <memory>
#include <vector>

#include "camera/camera.h"
#include "types/mat4x4.h"
#include "types/vec3.h"

class ShadowMap {
public:
    bool allocateShadowMap(int powerOfTwo);
    Mat4x4 getLightSpaceMatrix(const Vec3 &lightPos, const Vec3 &lightDir, float outerCutoffAngle,
                               float nearPlane, float farPlane);

    uint32_t getDepthMapFbo() const {
        return depthMapFbo_;
    }

    uint32_t getDepthMap() const {
        return depthMap_;
    }

    int getResolution() const {
        return resolution_;
    }

private:
    uint32_t depthMapFbo_;
    uint32_t depthMap_;
    int resolution_ = 0;
};

#endif