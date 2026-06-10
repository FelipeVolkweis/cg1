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
    cascadedShadowShaderProgram_ = std::make_shared<Shader>(SHDW_VERT, SHDW_FRAG, SHDW_GEOM);
    spotlightShadowShaderProgram_ = std::make_shared<Shader>(SHDW_VERT, SHDW_FRAG);
}

bool Renderer::initialize() {
    bool mainOk = mainShaderProgram_->initialize();
    bool shadowOk = cascadedShadowShaderProgram_->initialize();
    bool spotlightShadowOk = spotlightShadowShaderProgram_->initialize();

    if (!mainOk || !shadowOk || !spotlightShadowOk)
        return false;

    uint32_t mainBlockIndex =
        glGetUniformBlockIndex(mainShaderProgram_->getId(), "LightSpaceMatrices");
    if (mainBlockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(mainShaderProgram_->getId(), mainBlockIndex, 0);
    }

    uint32_t shadowBlockIndex =
        glGetUniformBlockIndex(cascadedShadowShaderProgram_->getId(), "LightSpaceMatrices");
    if (shadowBlockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(cascadedShadowShaderProgram_->getId(), shadowBlockIndex, 0);
    }

    return true;
}

void Renderer::render() {
    int w, h;
    glfwGetFramebufferSize(window_, &w, &h);

    cascadedShadowPass();
    spotlightShadowPass();
    pointLightShadowPass();

    glViewport(0, 0, w, h);

    mainPass();
}

void Renderer::mainPass() {
    if (auto activeCamera = activeCamera_.lock()) {
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

void Renderer::cascadedShadowPass() {
    if (directionalLight_) {
        int w, h;
        glfwGetFramebufferSize(window_, &w, &h);
        float aspect = static_cast<float>(w) / static_cast<float>(h);

        if (auto activeCamera = activeCamera_.lock()) {
            cascadedShadowShaderProgram_->use();
            directionalLight_->setCamera(activeCamera);
            directionalLight_->setFrameBufferAspect(aspect);
            directionalLight_->updateShadows();

            auto &shadow = directionalLight_->getShadow();
            int res = shadow.getResolution();

            if (res > 0) {
                glBindFramebuffer(GL_FRAMEBUFFER, shadow.getDepthMapFbo());
                glViewport(0, 0, res, res);
                glClear(GL_DEPTH_BUFFER_BIT);
                glCullFace(GL_FRONT);

                cascadedShadowShaderProgram_->use();
                for (auto &renderable : renderables_) {
                    auto &model = transforms_[renderable.first].getTransformationMatrix();
                    cascadedShadowShaderProgram_->setMat4x4("model", model);
                    renderable.second->renderShadow();
                }

                glCullFace(GL_BACK);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D_ARRAY, shadow.getDepthMap());
            }
        }
    }
}

void Renderer::spotlightShadowPass() {
    if (!spotlightShadowArrayInitialized_ || spotlights_.empty())
        return;

    int res = spotlightShadowArray_.getResolution();
    if (res <= 0)
        return;

    for (auto &pair : spotlights_) {
        auto &sl = pair.second;
        if (!sl)
            continue;

        sl->updateShadows();
        int layer = sl->getIndex();

        if (layer >= spotlightShadowArray_.getMaxLayers())
            continue;

        glBindFramebuffer(GL_FRAMEBUFFER, spotlightShadowArray_.getFboForLayer(layer));
        glViewport(0, 0, res, res);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);

        spotlightShadowShaderProgram_->use();
        for (auto &renderable : renderables_) {
            auto &modelMatrix = transforms_[renderable.first].getTransformationMatrix();
            Mat4x4 mvp = sl->getLightSpaceMatrix() * modelMatrix;
            spotlightShadowShaderProgram_->setMat4x4("model", mvp);
            renderable.second->renderShadow();
        }

        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D_ARRAY, spotlightShadowArray_.getDepthMap());
}

void Renderer::pointLightShadowPass() {
    if (!pointLightShadowArrayInitialized_ || pointLights_.empty())
        return;

    int res = pointLightShadowArray_.getResolution();
    if (res <= 0)
        return;

    for (auto &pair : pointLights_) {
        auto &pl = pair.second;
        if (!pl)
            continue;

        pl->updateShadows(pointLightShadowArray_);
        int lightIndex = pl->getIndex();

        if (lightIndex >= pointLightShadowArray_.getMaxLights())
            continue;

        const auto &matrices = pl->getLightSpaceMatrices();

        for (int face = 0; face < 6; ++face) {
            glBindFramebuffer(GL_FRAMEBUFFER,
                              pointLightShadowArray_.getFboForFace(lightIndex, face));
            glViewport(0, 0, res, res);
            glClear(GL_DEPTH_BUFFER_BIT);
            glCullFace(GL_FRONT);

            spotlightShadowShaderProgram_->use();
            for (auto &renderable : renderables_) {
                auto &modelMatrix = transforms_[renderable.first].getTransformationMatrix();
                Mat4x4 mvp = matrices[face] * modelMatrix;
                spotlightShadowShaderProgram_->setMat4x4("model", mvp);
                renderable.second->renderShadow();
            }

            glCullFace(GL_BACK);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, pointLightShadowArray_.getDepthMap());
}

void Renderer::renderDirectionalLight() {
    if (directionalLight_) {
        mainShaderProgram_->setInt("hasDirectionalLight", 1);
        directionalLight_->render();
    } else {
        mainShaderProgram_->setInt("hasDirectionalLight", 0);
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
    mainShaderProgram_->setInt("pointLightShadowMap", 4);
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
    mainShaderProgram_->setInt("spotlightShadowMap", 3);
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

        // Allocate shared shadow array on first point light
        if (!pointLightShadowArrayInitialized_) {
            pointLightShadowArrayInitialized_ = pointLightShadowArray_.allocate(10, MAX_POINT_LIGHTS);
        }
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

        // Allocate shared shadow array on first spotlight
        if (!spotlightShadowArrayInitialized_) {
            spotlightShadowArrayInitialized_ = spotlightShadowArray_.allocate(10, MAX_SPOTLIGHTS);
        }

        spotlight->setIndex(spotlights_.size());
        spotlight->setShaderProgram(mainShaderProgram_);
        spotlight->initializeOnGPU();
    }
    spotlights_[id] = spotlight;
}
