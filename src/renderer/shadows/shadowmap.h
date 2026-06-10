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

class SpotlightShadowArray {
public:
    bool allocate(int powerOfTwo, int maxLayers);

    uint32_t getDepthMap() const {
        return depthMap_;
    }

    uint32_t getFboForLayer(int layer) const {
        return layerFbos_[layer];
    }

    int getResolution() const {
        return resolution_;
    }

    int getMaxLayers() const {
        return maxLayers_;
    }

private:
    uint32_t depthMap_ = 0;
    std::vector<uint32_t> layerFbos_;
    int resolution_ = 0;
    int maxLayers_ = 0;
};

#endif