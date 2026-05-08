#ifndef SHADOW_H
#define SHADOW_H

#include <cstdint>
#include <memory>
#include <vector>

#include "camera/camera.h"
#include "types/mat4x4.h"
#include "types/vec3.h"

class Shadow {
public:
    std::vector<float> getShadowCascadeLevels(int levels, float startingDenominator, float zFar);
    bool allocateShadowMap(int powerOfTwo);
    std::vector<Mat4x4> getLightSpaceMatrices(std::shared_ptr<Camera> camera,
                                              float frameBufferAspect, const Mat4x4 &cameraView,
                                              const Vec3 &lightDirection);

private:
    uint32_t depthMapFbo_;
    uint32_t depthMap_;

    std::vector<float> shadowCascadeLevels_;

    int levels_;
    float startingDenominator_;
    float zFar_;
};

#endif