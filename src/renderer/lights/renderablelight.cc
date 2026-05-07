#include "renderablelight.h"

#include <glad/glad.h>

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

void RenderableDirectionalLight::initializeOnGPU() {}

void RenderableDirectionalLight::render() {
    if (!light_)
        return;

    shaderProgram_->setVec3("directionalLight.direction", light_->getDirection());
    shaderProgram_->setVec3("directionalLight.ambient", light_->getAmbient());
    shaderProgram_->setVec3("directionalLight.diffuse", light_->getDiffuse());
    shaderProgram_->setVec3("directionalLight.specular", light_->getSpecular());
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
