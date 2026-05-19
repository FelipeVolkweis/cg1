#include "shadowmap.h"

#include <glad/glad.h>

#include "math/projections/projection.h"
#include "utils/logger.h"

bool ShadowMap::allocateShadowMap(int powerOfTwo) {
    resolution_ = 1 << powerOfTwo;

    glGenFramebuffers(1, &depthMapFbo_);
    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D, depthMap_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution_, resolution_, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    constexpr float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

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
    glBindTexture(GL_TEXTURE_2D, depthMap_);

    return true;
}

Mat4x4 ShadowMap::getLightSpaceMatrix(const Vec3 &lightPos, const Vec3 &lightDir,
                                      float outerCutoffAngle, float nearPlane, float farPlane) {
    float fov = outerCutoffAngle * 2.0f;
    float aspect = 1.0f; // shadowMapRes / shadowMapRes = 1

    Mat4x4 lightProjection = Projection::getPerspective(fov, aspect, nearPlane, farPlane);
    Vec3 up = (std::abs(lightDir.y()) > 0.99f) ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0f, 1.0f, 0.0f);
    Mat4x4 lightView = Projection::getLookAt(lightPos, lightPos + lightDir, up);

    return lightProjection * lightView;
}