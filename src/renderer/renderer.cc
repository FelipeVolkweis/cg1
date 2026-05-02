#include "renderer.h"

#include <glad/glad.h>

#include "shapes/baseshape.h"
#include "utils/logger.h"

const char *vertexShaderSource = R"(
#version 330 core
attribute vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
void main() {
    FragColor = vec4(objectColor, 1.0f);
}
)";

Renderer::~Renderer() {
    if (vao_)
        glDeleteVertexArrays(1, &vao_);
    if (vbo_)
        glDeleteBuffers(1, &vbo_);
    if (shaderProgram_)
        glDeleteProgram(shaderProgram_);
}

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

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

void Renderer::render() {
    if (auto activeCamera = activeCamera_.lock()) {
        glUseProgram(shaderProgram_);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int viewLoc = glGetUniformLocation(shaderProgram_, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, activeCamera->lookAt().data());

        int projLoc = glGetUniformLocation(shaderProgram_, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, activeCamera->perspective().data());

        for (const RenderItem &item : items_) {
            const std::vector<Vec3> &vertices = item.shape->getVertices();
            if (vertices.empty())
                continue;

            const Mat4x4 &model = item.transform.getTransformationMatrix();
            const Vec3 &color = item.shape->getColor();

            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(),
                         GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
            glEnableVertexAttribArray(0);

            int modelLoc = glGetUniformLocation(shaderProgram_, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

            int colorLoc = glGetUniformLocation(shaderProgram_, "objectColor");
            glUniform3f(colorLoc, color.x(), color.y(), color.z());

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
    }
}