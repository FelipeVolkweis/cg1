#include "cascadedshadow.h"

#include <glad/glad.h>

#include <cmath>
#include <memory>

#include "camera/camera.h"
#include "math/projections/projection.h"
#include "types/mat4x4.h"
#include "utils/logger.h"

// https://learnopengl.com/Guest-Articles/2021/CSM

std::vector<float> CascadedShadow::getShadowCascadeLevels(int levels, float zNear, float zFar,
                                                          float splitLambda) {
    if (!shadowCascadeLevels_.empty() && levels == levels_ && std::abs(zNear_ - zNear) <= 1e-3f &&
        std::abs(zFar_ - zFar) <= 1e-3f && std::abs(splitLambda_ - splitLambda) <= 1e-3f) {
        return shadowCascadeLevels_;
    }

    levels_ = levels;
    zNear_ = zNear;
    zFar_ = zFar;
    splitLambda_ = splitLambda;

    shadowCascadeLevels_.clear();
    const int cascadeCount = levels + 1;
    for (int i = 1; i < cascadeCount; ++i) {
        float ratio = static_cast<float>(i) / static_cast<float>(cascadeCount);
        float logarithmic = zNear * std::pow(zFar / zNear, ratio);
        float uniform = zNear + (zFar - zNear) * ratio;
        shadowCascadeLevels_.push_back(splitLambda * logarithmic + (1.0f - splitLambda) * uniform);
    }

    return shadowCascadeLevels_;
}

bool CascadedShadow::allocateShadowMap(int powerOfTwo, int levels) {
    resolution_ = 1 << powerOfTwo;

    glGenFramebuffers(1, &depthMapFbo_);
    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap_);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, resolution_, resolution_,
                 levels + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    constexpr float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Logger::Error(" Framebuffer is not complete!");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap_);

    return true;
}

std::vector<Mat4x4> CascadedShadow::getLightSpaceMatrices(std::shared_ptr<Camera> camera,
                                                          float frameBufferAspect,
                                                          const Mat4x4 &cameraView,
                                                          const Vec3 &lightDirection) {
    std::vector<Mat4x4> ret;
    if (shadowCascadeLevels_.size() < 1)
        return ret;

    for (size_t i = 0; i < shadowCascadeLevels_.size() + 1; ++i) {
        float zNear, zFar;
        if (i == 0) {
            zNear = camera->getZNear();
            zFar = shadowCascadeLevels_[i];
        } else if (i < shadowCascadeLevels_.size()) {
            zNear = shadowCascadeLevels_[i - 1];
            zFar = shadowCascadeLevels_[i];
        } else {
            zNear = shadowCascadeLevels_[i - 1];
            zFar = zFar_;
        }

        Mat4x4 cameraProjection =
            Projection::getPerspective(camera->getFov(), frameBufferAspect, zNear, zFar);
        ret.push_back(
            Projection::getLightSpaceMatrix(cameraProjection, cameraView, lightDirection));
    }
    return ret;
}
