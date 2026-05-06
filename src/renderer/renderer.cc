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

    viewLocation_ = glGetUniformLocation(shaderProgram_, "view");
    projectionLocation_ = glGetUniformLocation(shaderProgram_, "projection");

    viewPosLocation_ = glGetUniformLocation(shaderProgram_, "viewPos");

    dl_directionLocation_ = glGetUniformLocation(shaderProgram_, "directionalLight.direction");
    dl_ambientLocation_ = glGetUniformLocation(shaderProgram_, "directionalLight.ambient");
    dl_diffuseLocation_ = glGetUniformLocation(shaderProgram_, "directionalLight.diffuse");
    dl_specularLocation_ = glGetUniformLocation(shaderProgram_, "directionalLight.specular");

    // pointLightsLocation_ = glGetUniformLocation(shaderProgram_, "pointLights");
    // spolightsLocation_ = glGetUniformLocation(shaderProgram_, "spolights");

    numPointLightsLocation_ = glGetUniformLocation(shaderProgram_, "numPointLights");
    numSpotlightsLocation_ = glGetUniformLocation(shaderProgram_, " numSpolights");

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

        glUniform1i(numPointLightsLocation_, 0);
        glUniform1i(numSpotlightsLocation_, 0);
        glUniform3f(dl_directionLocation_, -0.2f, -1.0f, -0.3f);
        glUniform3f(dl_ambientLocation_, 0.05f, 0.05f, 0.05f);
        glUniform3f(dl_diffuseLocation_, 0.4f, 0.4f, 0.4f);
        glUniform3f(dl_specularLocation_, 0.5f, 0.5f, 0.5f);

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