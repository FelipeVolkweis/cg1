#include "renderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "shapes/baseshape.h"
#include "textures/texture.h"
#include "utils/fileutils.h"
#include "utils/logger.h"

#define MAIN_VERT "shaders/renderer.vert"
#define MAIN_FRAG "shaders/renderer.frag"

#define SHDW_VERT "shaders/shadowmap.vert"
#define SHDW_FRAG "shaders/shadowmap.frag"
#define SHDW_GEOM "shaders/shadowmap.geom"

Renderer::Renderer() {
    mainShaderProgram_ = std::make_shared<Shader>(MAIN_VERT, MAIN_FRAG);
    shadowShaderProgram_ = std::make_shared<Shader>(SHDW_VERT, SHDW_FRAG, SHDW_GEOM);
}

bool Renderer::initialize() {
    bool mainOk = mainShaderProgram_->initialize();
    bool shadowOk = shadowShaderProgram_->initialize();

    if (!mainOk || !shadowOk)
        return false;

    return true;
}

void Renderer::render() {
    if (auto activeCamera = activeCamera_.lock()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (skybox_) {
            skybox_->render(activeCamera->lookAt(), activeCamera->perspective());
        }

        const auto &view = activeCamera->lookAt();
        const auto &projection = activeCamera->perspective();
        const auto &viewPos = activeCamera->getPosition();

        mainShaderProgram_->use();
        mainShaderProgram_->setMat4x4("view", view);
        mainShaderProgram_->setMat4x4("projection", projection);
        mainShaderProgram_->setVec3("viewPos", viewPos);
        mainShaderProgram_->setFloat("zFar", activeCamera->getZFar());

        renderDirectionalLight();
        renderPointLights();
        renderSpotlights();

        renderOpaqueMeshes();
        renderTransluscenteMeshes();
    }
}

void Renderer::renderDirectionalLight() {
    if (directionalLight_) {
        auto activeCamera = activeCamera_.lock();
        if (activeCamera) {
            directionalLight_->setCamera(activeCamera);
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
            float aspect = static_cast<float>(w) / static_cast<float>(h);
        }
        directionalLight_->render();
    }
}

void Renderer::renderPointLights() {
    int plCount = 0;
    for (const auto &pair : pointLights_) {
        if (plCount >= MAX_POINT_LIGHTS)
            break;
        auto pl = pair.second;
        if (!pl)
            continue;
        pl->render();
        plCount++;
    }
    mainShaderProgram_->setInt("numPointLights", plCount);
}

void Renderer::renderSpotlights() {
    int slCount = 0;
    for (const auto &pair : spotlights_) {
        if (slCount >= MAX_SPOTLIGHTS)
            break;
        auto sl = pair.second;
        if (!sl)
            continue;
        sl->render();
        slCount++;
    }
    mainShaderProgram_->setInt("numSpotlights", slCount);
}

void Renderer::renderOpaqueMeshes() {
    for (auto &renderable : renderables_) {
        auto &model = transforms_[renderable.first].getTransformationMatrix();
        renderable.second->render(model, false);
    }
}

void Renderer::renderTransluscenteMeshes() {

    for (auto &renderable : renderables_) {
        auto &model = transforms_[renderable.first].getTransformationMatrix();
        renderable.second->render(model, true);
    }
}

void Renderer::addRenderable(uint64_t id, std::shared_ptr<RenderableMesh> renderable) {
    if (renderables_.find(id) != renderables_.end())
        return;

    renderable->setShaderProgram(mainShaderProgram_);
    renderable->initializeOnGPU();
    renderables_[id] = renderable;
}

void Renderer::setDirectionalLight(std::shared_ptr<RenderableDirectionalLight> directionalLight) {
    if (!directionalLight)
        return;
    if (!directionalLight_) {
        directionalLight->setShaderProgram(mainShaderProgram_);
        directionalLight->initializeOnGPU();
        directionalLight_ = directionalLight;
    }
}

void Renderer::addPointLight(uint64_t id, std::shared_ptr<RenderablePointLight> pointLight) {
    if (!pointLight)
        return;

    if (pointLights_.find(id) == pointLights_.end()) {
        if (pointLights_.size() >= MAX_POINT_LIGHTS) {
            Logger::Warn("Maximum Point Lights Reached");
            return;
        }
        pointLight->setIndex(pointLights_.size());
        pointLight->setShaderProgram(mainShaderProgram_);
        pointLight->initializeOnGPU();
    }
    pointLights_[id] = pointLight;
}

void Renderer::addSpotlight(uint64_t id, std::shared_ptr<RenderableSpotlight> spotlight) {
    if (!spotlight)
        return;

    if (spotlights_.find(id) == spotlights_.end()) {
        if (spotlights_.size() >= MAX_SPOTLIGHTS) {
            Logger::Warn("Maximum Spotlights Reached");
            return;
        }
        spotlight->setIndex(spotlights_.size());
        spotlight->setShaderProgram(mainShaderProgram_);
        spotlight->initializeOnGPU();
    }
    spotlights_[id] = spotlight;
}
