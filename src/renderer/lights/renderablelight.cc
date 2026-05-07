#include "renderablelight.h"

#include <glad/glad.h>

#include "math/projections/projection.h"

RenderablePointLight::RenderablePointLight(uint64_t id) : id_(id) {}

void RenderablePointLight::initializeOnGPU() {
    std::string prefix = "pointLights[" + std::to_string(index_) + "].";
    locations_ = {glGetUniformLocation(shaderProgram_, (prefix + "position").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "constant").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "linear").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "quadratic").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "ambient").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "diffuse").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "specular").c_str())};
}

void RenderablePointLight::render() {
    if (!light_)
        return;

    glUniform3f(locations_.position, light_->getPosition().x(), light_->getPosition().y(),
                light_->getPosition().z());
    glUniform1f(locations_.constant, light_->getConstant());
    glUniform1f(locations_.linear, light_->getLinear());
    glUniform1f(locations_.quadratic, light_->getQuadratic());
    glUniform3f(locations_.ambient, light_->getAmbient().x(), light_->getAmbient().y(),
                light_->getAmbient().z());
    glUniform3f(locations_.diffuse, light_->getDiffuse().x(), light_->getDiffuse().y(),
                light_->getDiffuse().z());
    glUniform3f(locations_.specular, light_->getSpecular().x(), light_->getSpecular().y(),
                light_->getSpecular().z());
}

RenderableDirectionalLight::RenderableDirectionalLight(uint64_t id) : id_(id) {}

void RenderableDirectionalLight::initializeOnGPU() {
    locations_.direction = glGetUniformLocation(shaderProgram_, "directionalLight.direction");
    locations_.ambient = glGetUniformLocation(shaderProgram_, "directionalLight.ambient");
    locations_.diffuse = glGetUniformLocation(shaderProgram_, "directionalLight.diffuse");
    locations_.specular = glGetUniformLocation(shaderProgram_, "directionalLight.specular");
    locations_.lightSpaceMatrix =
        glGetUniformLocation(shaderProgram_, "directionalLightSpaceMatrix");
}

void RenderableDirectionalLight::render() {
    if (!light_)
        return;

    glUniform3f(locations_.direction, light_->getDirection().x(), light_->getDirection().y(),
                light_->getDirection().z());
    glUniform3f(locations_.ambient, light_->getAmbient().x(), light_->getAmbient().y(),
                light_->getAmbient().z());
    glUniform3f(locations_.diffuse, light_->getDiffuse().x(), light_->getDiffuse().y(),
                light_->getDiffuse().z());
    glUniform3f(locations_.specular, light_->getSpecular().x(), light_->getSpecular().y(),
                light_->getSpecular().z());
    // glUniformMatrix4fv(locations_.lightSpaceMatrix, 1, GL_FALSE, view.data());
}

RenderableSpotlight::RenderableSpotlight(uint64_t id) : id_(id) {}

void RenderableSpotlight::initializeOnGPU() {
    std::string prefix = "spotlights[" + std::to_string(index_) + "].";
    locations_ = {glGetUniformLocation(shaderProgram_, (prefix + "position").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "direction").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "cutoff").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "outerCutoff").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "constant").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "linear").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "quadratic").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "ambient").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "diffuse").c_str()),
                  glGetUniformLocation(shaderProgram_, (prefix + "specular").c_str())};
}

void RenderableSpotlight::render() {
    if (!light_)
        return;
    glUniform3f(locations_.position, light_->getPosition().x(), light_->getPosition().y(),
                light_->getPosition().z());
    glUniform3f(locations_.direction, light_->getDirection().x(), light_->getDirection().y(),
                light_->getDirection().z());
    glUniform1f(locations_.cutoff, std::cos(light_->getCutoff()));
    glUniform1f(locations_.outerCutoff, std::cos(light_->getOuterCutoff()));
    glUniform1f(locations_.constant, light_->getConstant());
    glUniform1f(locations_.linear, light_->getLinear());
    glUniform1f(locations_.quadratic, light_->getQuadratic());
    glUniform3f(locations_.ambient, light_->getAmbient().x(), light_->getAmbient().y(),
                light_->getAmbient().z());
    glUniform3f(locations_.diffuse, light_->getDiffuse().x(), light_->getDiffuse().y(),
                light_->getDiffuse().z());
    glUniform3f(locations_.specular, light_->getSpecular().x(), light_->getSpecular().y(),
                light_->getSpecular().z());
}
