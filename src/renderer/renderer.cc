#include "renderer.h"

#include <glad/glad.h>

#include "shapes/baseshape.h"
#include "texture.h"
#include "utils/logger.h"

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aColor;

out vec2 TexCoord;
out vec4 VertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoords;
    VertColor = aColor;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertColor;

uniform sampler2D texture_diffuse;

void main() {
    vec4 texColor = texture(texture_diffuse, TexCoord);

    FragColor = texColor * VertColor;
}
)";

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
    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

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

        glUseProgram(shaderProgram_);
        int viewLoc = glGetUniformLocation(shaderProgram_, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());

        int projLoc = glGetUniformLocation(shaderProgram_, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());

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