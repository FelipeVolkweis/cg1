#include "renderer.h"

#include <glad/glad.h>

#include "shapes/baseshape.h"
#include "textures/texture.h"
#include "utils/fileutils.h"
#include "utils/logger.h"

// !!SYNC THIS WITH THE RENDERER.FRAG SHADER!!
#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 16

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

    directionalLightLocations_.direction =
        glGetUniformLocation(shaderProgram_, "directionalLight.direction");
    directionalLightLocations_.ambient =
        glGetUniformLocation(shaderProgram_, "directionalLight.ambient");
    directionalLightLocations_.diffuse =
        glGetUniformLocation(shaderProgram_, "directionalLight.diffuse");
    directionalLightLocations_.specular =
        glGetUniformLocation(shaderProgram_, "directionalLight.specular");

    pointLightLocations_.clear();
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
        pointLightLocations_.push_back(
            {glGetUniformLocation(shaderProgram_, (prefix + "position").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "constant").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "linear").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "quadratic").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "ambient").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "diffuse").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "specular").c_str())});
    }

    spotlightLocations_.clear();
    for (int i = 0; i < MAX_SPOTLIGHTS; ++i) {
        std::string prefix = "spotlights[" + std::to_string(i) + "].";
        spotlightLocations_.push_back(
            {glGetUniformLocation(shaderProgram_, (prefix + "position").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "direction").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "cutoff").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "outerCutoff").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "constant").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "linear").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "quadratic").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "ambient").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "diffuse").c_str()),
             glGetUniformLocation(shaderProgram_, (prefix + "specular").c_str())});
    }

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
            glUniform3f(directionalLightLocations_.direction, directionalLight_->getDirection().x(),
                        directionalLight_->getDirection().y(),
                        directionalLight_->getDirection().z());
            glUniform3f(directionalLightLocations_.ambient, directionalLight_->getAmbient().x(),
                        directionalLight_->getAmbient().y(), directionalLight_->getAmbient().z());
            glUniform3f(directionalLightLocations_.diffuse, directionalLight_->getDiffuse().x(),
                        directionalLight_->getDiffuse().y(), directionalLight_->getDiffuse().z());
            glUniform3f(directionalLightLocations_.specular, directionalLight_->getSpecular().x(),
                        directionalLight_->getSpecular().y(), directionalLight_->getSpecular().z());
        }

        int plCount = 0;
        for (const auto &pair : pointLights_) {
            if (plCount >= MAX_POINT_LIGHTS)
                break;
            auto pl = pair.second;
            glUniform3f(pointLightLocations_[plCount].position, pl->getPosition().x(),
                        pl->getPosition().y(), pl->getPosition().z());
            glUniform1f(pointLightLocations_[plCount].constant, pl->getConstant());
            glUniform1f(pointLightLocations_[plCount].linear, pl->getLinear());
            glUniform1f(pointLightLocations_[plCount].quadratic, pl->getQuadratic());
            glUniform3f(pointLightLocations_[plCount].ambient, pl->getAmbient().x(),
                        pl->getAmbient().y(), pl->getAmbient().z());
            glUniform3f(pointLightLocations_[plCount].diffuse, pl->getDiffuse().x(),
                        pl->getDiffuse().y(), pl->getDiffuse().z());
            glUniform3f(pointLightLocations_[plCount].specular, pl->getSpecular().x(),
                        pl->getSpecular().y(), pl->getSpecular().z());
            plCount++;
        }
        glUniform1i(numPointLightsLocation_, plCount);

        int slCount = 0;
        for (const auto &pair : spotlights_) {
            if (slCount >= MAX_SPOTLIGHTS)
                break;
            auto sl = pair.second;
            glUniform3f(spotlightLocations_[slCount].position, sl->getPosition().x(),
                        sl->getPosition().y(), sl->getPosition().z());
            glUniform3f(spotlightLocations_[slCount].direction, sl->getDirection().x(),
                        sl->getDirection().y(), sl->getDirection().z());
            glUniform1f(spotlightLocations_[slCount].cutoff, std::cos(sl->getCutoff()));
            glUniform1f(spotlightLocations_[slCount].outerCutoff, std::cos(sl->getOuterCutoff()));
            glUniform1f(spotlightLocations_[slCount].constant, sl->getConstant());
            glUniform1f(spotlightLocations_[slCount].linear, sl->getLinear());
            glUniform1f(spotlightLocations_[slCount].quadratic, sl->getQuadratic());
            glUniform3f(spotlightLocations_[slCount].ambient, sl->getAmbient().x(),
                        sl->getAmbient().y(), sl->getAmbient().z());
            glUniform3f(spotlightLocations_[slCount].diffuse, sl->getDiffuse().x(),
                        sl->getDiffuse().y(), sl->getDiffuse().z());
            glUniform3f(spotlightLocations_[slCount].specular, sl->getSpecular().x(),
                        sl->getSpecular().y(), sl->getSpecular().z());
            slCount++;
        }
        glUniform1i(numSpotlightsLocation_, slCount);

        for (auto &renderable : renderables_) {
            auto &model = transforms_[renderable.first].getTransformationMatrix();
            renderable.second.render(model, false);
        }

        for (auto &renderable : renderables_) {
            auto &model = transforms_[renderable.first].getTransformationMatrix();
            renderable.second.render(model, true);
        }
    }
}

void Renderer::addRenderable(Renderable renderable) {
    renderable.setShaderProgram(shaderProgram_);
    renderable.initializeOnGPU();
    renderables_[renderable.getId()] = std::move(renderable);
}

void Renderer::addPointLight(uint64_t id, std::shared_ptr<PointLight> pointLight) {
    if (pointLights_.size() > MAX_POINT_LIGHTS) {
        Logger::Warn("Maximum Point Lights Reached");
        return;
    }
    pointLights_[id] = pointLight;
}

void Renderer::addSpotlight(uint64_t id, std::shared_ptr<Spotlight> spotlight) {
    if (spotlights_.size() > MAX_SPOTLIGHTS) {
        Logger::Warn("Maximum Spotlights Reached");
        return;
    }
    spotlights_[id] = spotlight;
}