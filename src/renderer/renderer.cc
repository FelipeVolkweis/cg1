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

    uint32_t mainBlockIndex = glGetUniformBlockIndex(mainShaderProgram_->getId(), "LightSpaceMatrices");
    if (mainBlockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(mainShaderProgram_->getId(), mainBlockIndex, 0);
    }

    uint32_t shadowBlockIndex = glGetUniformBlockIndex(shadowShaderProgram_->getId(), "LightSpaceMatrices");
    if (shadowBlockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(shadowShaderProgram_->getId(), shadowBlockIndex, 0);
    }

    return true;
}

void Renderer::render() {
    if (auto activeCamera = activeCamera_.lock()) {
        // --- Shadow Pass ---
        if (directionalLight_) {
            shadowShaderProgram_->use();
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
            float aspect = static_cast<float>(w) / static_cast<float>(h);
            directionalLight_->setCamera(activeCamera);
            directionalLight_->setFrameBufferAspect(aspect);
            directionalLight_->updateShadows();

            auto& shadow = directionalLight_->getShadow();
            int res = shadow.getResolution();
            
            if (res > 0) {
                glBindFramebuffer(GL_FRAMEBUFFER, shadow.getDepthMapFbo());
                glViewport(0, 0, res, res);
                glClear(GL_DEPTH_BUFFER_BIT);
                glCullFace(GL_FRONT);
                
                shadowShaderProgram_->use();
                for (auto &renderable : renderables_) {
                    auto &model = transforms_[renderable.first].getTransformationMatrix();
                    shadowShaderProgram_->setMat4x4("model", model);
                    renderable.second->renderShadow();
                }
                
                glCullFace(GL_BACK);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                
                glViewport(0, 0, w, h);
                
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D_ARRAY, shadow.getDepthMap());
            }
        }
        // -------------------

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (skybox_) {
            skybox_->render(activeCamera->lookAt(), activeCamera->perspective());
        }

        mainShaderProgram_->use();
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
