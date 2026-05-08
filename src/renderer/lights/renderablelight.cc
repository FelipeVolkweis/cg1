#include "renderablelight.h"

#include <glad/glad.h>

#include <iostream>

#include "math/projections/projection.h"

RenderablePointLight::RenderablePointLight(uint64_t id) : id_(id) {}

void RenderablePointLight::initializeOnGPU() {}

void RenderablePointLight::render() {
    if (!light_)
        return;

    std::string prefix = "pointLights[" + std::to_string(index_) + "].";
    shaderProgram_->setVec3(prefix + "position", light_->getPosition());
    shaderProgram_->setFloat(prefix + "constant", light_->getConstant());
    shaderProgram_->setFloat(prefix + "linear", light_->getLinear());
    shaderProgram_->setFloat(prefix + "quadratic", light_->getQuadratic());
    shaderProgram_->setVec3(prefix + "ambient", light_->getAmbient());
    shaderProgram_->setVec3(prefix + "diffuse", light_->getDiffuse());
    shaderProgram_->setVec3(prefix + "specular", light_->getSpecular());
}

RenderableDirectionalLight::RenderableDirectionalLight(uint64_t id) : id_(id) {}

void RenderableDirectionalLight::initializeOnGPU() {
    shadow_.allocateShadowMap(10);
    glGenBuffers(1, &ubo_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4x4) * 16, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderableDirectionalLight::render() {
    if (!light_)
        return;

    shaderProgram_->setVec3("directionalLight.direction", light_->getDirection());
    shaderProgram_->setVec3("directionalLight.ambient", light_->getAmbient());
    shaderProgram_->setVec3("directionalLight.diffuse", light_->getDiffuse());
    shaderProgram_->setVec3("directionalLight.specular", light_->getSpecular());

    if (!camera_)
        return;

    shaderProgram_->setInt("shadowMap", 2);
    const auto &shadowCascadeLevels = shadow_.getShadowCascadeLevels(3, 50, camera_->getZFar());
    const auto lightMatrices = shadow_.getLightSpaceMatrices(
        camera_, frameBufferAspect_, camera_->lookAt(), light_->getDirection());
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
    for (size_t i = 0; i < lightMatrices.size(); ++i) {
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(Mat4x4), sizeof(Mat4x4), &lightMatrices[i]);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    shaderProgram_->setInt("cascadeCount", shadowCascadeLevels.size());
    for (size_t i = 0; i < shadowCascadeLevels.size(); ++i) {
        shaderProgram_->setFloat("cascadePlaneDistances[" + std::to_string(i) + "]",
                                 shadowCascadeLevels[i]);
    }
}

RenderableSpotlight::RenderableSpotlight(uint64_t id) : id_(id) {}

void RenderableSpotlight::initializeOnGPU() {}

void RenderableSpotlight::render() {
    if (!light_)
        return;

    std::string prefix = "spotlights[" + std::to_string(index_) + "].";
    shaderProgram_->setVec3(prefix + "position", light_->getPosition());
    shaderProgram_->setVec3(prefix + "direction", light_->getDirection());
    shaderProgram_->setFloat(prefix + "cutoff", std::cos(light_->getCutoff()));
    shaderProgram_->setFloat(prefix + "outerCutoff", std::cos(light_->getOuterCutoff()));
    shaderProgram_->setFloat(prefix + "constant", light_->getConstant());
    shaderProgram_->setFloat(prefix + "linear", light_->getLinear());
    shaderProgram_->setFloat(prefix + "quadratic", light_->getQuadratic());
    shaderProgram_->setVec3(prefix + "ambient", light_->getAmbient());
    shaderProgram_->setVec3(prefix + "diffuse", light_->getDiffuse());
    shaderProgram_->setVec3(prefix + "specular", light_->getSpecular());
}
