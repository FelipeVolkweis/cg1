#ifndef SHADOW_H
#define SHADOW_H

#include <cstdint>
#include <vector>

class Shadow {
public:
    bool allocateShadowMap(int powerOfTwo);

private:
    uint32_t depthMapFbo_;
    uint32_t depthMap_;

    std::vector<float> shadowCascadeLevels_;
};

#endif