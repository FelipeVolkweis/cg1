#include "shadow.h"

#include <glad/glad.h>

#include <memory>

#include "camera/camera.h"
#include "math/projections/projection.h"
#include "types/mat4x4.h"
#include "utils/logger.h"

// https://learnopengl.com/Guest-Articles/2021/CSM

std::vector<float> Shadow::getShadowCascadeLevels(int levels, float startingDenominator,
                                                  float zFar) {
    levels_ = levels;
    startingDenominator_ = startingDenominator;
    zFar_ = zFar;

    if (levels == levels_ && abs(startingDenominator_ - startingDenominator) <= 1e-3 &&
        abs(zFar_ - zFar) <= 1e-3) {
        return shadowCascadeLevels_;
    }
    shadowCascadeLevels_.clear();
    float den = startingDenominator;
    for (int i = 0; i < levels; i++) {
        if (den >= 1) {
            shadowCascadeLevels_.push_back(zFar / den);
        } else {
            break;
        }
        den /= 2;
    }

    return shadowCascadeLevels_;
}

bool Shadow::allocateShadowMap(int powerOfTwo) {
    int resolution = 1 << powerOfTwo;

    glGenFramebuffers(1, &depthMapFbo_);
    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap_);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, resolution, resolution,
                 int(shadowCascadeLevels_.size()) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

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

std::vector<Mat4x4> Shadow::getLightSpaceMatrices(std::shared_ptr<Camera> camera,
                                                  float frameBufferAspect, const Mat4x4 &cameraView,
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
            zFar = camera->getZFar();
        }

        Mat4x4 cameraProjection =
            Projection::getPerspective(camera->getFov(), frameBufferAspect, zNear, zFar);
        ret.push_back(
            Projection::getLightSpaceMatrix(cameraProjection, cameraView, lightDirection));
    }
    return ret;
}