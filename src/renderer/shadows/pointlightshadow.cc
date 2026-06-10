#include "pointlightshadow.h"

#include <glad/glad.h>

#include "math/projections/projection.h"
#include "utils/logger.h"

bool PointLightShadowArray::allocate(int powerOfTwo, int maxLights) {
    resolution_ = 1 << powerOfTwo;
    maxLights_ = maxLights;

    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, depthMap_);
    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT32F, resolution_, resolution_,
                 maxLights * 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    faceFbos_.resize(maxLights * 6);
    for (int light = 0; light < maxLights; ++light) {
        for (int face = 0; face < 6; ++face) {
            int idx = light * 6 + face;
            glGenFramebuffers(1, &faceFbos_[idx]);
            glBindFramebuffer(GL_FRAMEBUFFER, faceFbos_[idx]);
            glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap_, 0, idx);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE) {
                Logger::Error("Point light shadow FBO (light ", light, ", face ", face,
                              ") is not complete!");
                return false;
            }
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

std::array<Mat4x4, 6> PointLightShadowArray::getViewProjectionMatrices(const Vec3 &lightPos,
                                                                        float nearPlane,
                                                                        float farPlane) {
    // 90 degrees FOV, aspect 1:1
    Mat4x4 projection = Projection::getPerspective(90.0f, 1.0f, nearPlane, farPlane);

    std::array<Mat4x4, 6> matrices;

    matrices[0] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(1.0f, 0.0f, 0.0f),
                                                     Vec3(0.0f, -1.0f, 0.0f));
    matrices[1] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(-1.0f, 0.0f, 0.0f),
                                                     Vec3(0.0f, -1.0f, 0.0f));
    matrices[2] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(0.0f, 1.0f, 0.0f),
                                                     Vec3(0.0f, 0.0f, 1.0f));
    matrices[3] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(0.0f, -1.0f, 0.0f),
                                                     Vec3(0.0f, 0.0f, -1.0f));
    matrices[4] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(0.0f, 0.0f, 1.0f),
                                                     Vec3(0.0f, -1.0f, 0.0f));
    matrices[5] = projection * Projection::getLookAt(lightPos, lightPos + Vec3(0.0f, 0.0f, -1.0f),
                                                     Vec3(0.0f, -1.0f, 0.0f));

    return matrices;
}
