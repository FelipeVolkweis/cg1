#include "renderer.h"

#include <glad/glad.h>

#include "shapes/baseshape.h"
#include "textures/texture.h"
#include "utils/fileutils.h"
#include "utils/logger.h"

uint32_t Renderer::compileShader(uint32_t type, const char *source) {
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::Error("Error in shader compilation", infoLog);
    }
    return shader;
}

bool Renderer::initialize() {
    std::string vertexShaderSource = readFile("shaders/renderer.vert");
    std::string fragmentShaderSource = readFile("shaders/renderer.frag");

    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        Logger::Error("Error in shader program linkage\n", infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    viewLocation_ = glGetUniformLocation(shaderProgram_, "view");
    projectionLocation_ = glGetUniformLocation(shaderProgram_, "projection");

    viewPosLocation_ = glGetUniformLocation(shaderProgram_, "viewPos");

    numPointLightsLocation_ = glGetUniformLocation(shaderProgram_, "numPointLights");
    numSpotlightsLocation_ = glGetUniformLocation(shaderProgram_, "numSpotlights");

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

        glUseProgram(shaderProgram_);
        glUniformMatrix4fv(viewLocation_, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(projectionLocation_, 1, GL_FALSE, projection.data());
        glUniform3f(viewPosLocation_, viewPos.x(), viewPos.y(), viewPos.z());

        if (directionalLight_) {
            directionalLight_->render();
        }

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
        glUniform1i(numPointLightsLocation_, plCount);

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
        glUniform1i(numSpotlightsLocation_, slCount);

        for (auto &renderable : renderables_) {
            auto &model = transforms_[renderable.first].getTransformationMatrix();
            renderable.second->render(model, false);
        }

        for (auto &renderable : renderables_) {
            auto &model = transforms_[renderable.first].getTransformationMatrix();
            renderable.second->render(model, true);
        }
    }
}

void Renderer::addRenderable(uint64_t id, std::shared_ptr<RenderableMesh> renderable) {
    if (renderables_.find(id) != renderables_.end()) return;
    
    renderable->setShaderProgram(shaderProgram_);
    renderable->initializeOnGPU();
    renderables_[id] = renderable;
}

void Renderer::setDirectionalLight(std::shared_ptr<RenderableDirectionalLight> directionalLight) {
    if (!directionalLight)
        return;
    if (!directionalLight_) {
        directionalLight->setShaderProgram(shaderProgram_);
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
        pointLight->setShaderProgram(shaderProgram_);
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
        spotlight->setShaderProgram(shaderProgram_);
        spotlight->initializeOnGPU();
    }
    spotlights_[id] = spotlight;
}