#ifndef POINTLIGHTSHADOW_H
#define POINTLIGHTSHADOW_H

#include <array>
#include <cstdint>
#include <vector>

#include "types/mat4x4.h"
#include "types/vec3.h"

class PointLightShadowArray {
public:
    bool allocate(int powerOfTwo, int maxLights);

    std::array<Mat4x4, 6> getViewProjectionMatrices(const Vec3 &lightPos, float nearPlane,
                                                     float farPlane);

    uint32_t getDepthMap() const {
        return depthMap_;
    }

    uint32_t getFboForFace(int lightIndex, int face) const {
        return faceFbos_[lightIndex * 6 + face];
    }

    int getResolution() const {
        return resolution_;
    }

    int getMaxLights() const {
        return maxLights_;
    }

private:
    uint32_t depthMap_ = 0;
    std::vector<uint32_t> faceFbos_;
    int resolution_ = 0;
    int maxLights_ = 0;
};

#endif
