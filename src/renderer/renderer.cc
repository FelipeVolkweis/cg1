#include "renderer.h"

#include "shapes/baseshape.h"
#include "utils/logger.h"
#include <glad/glad.h>

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
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

    return true;
}

void Renderer::render() {
    glUseProgram(shaderProgram_);

    for (const RenderItem &item : items_) {
        const std::vector<Vec3> &vertices = item.shape->getVertices();
        if (vertices.empty())
            continue;

        const Mat4x4 &transform = item.transform.getTransformationMatrix();
        const Vec3 &color = item.shape->getColor();

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(),
                     GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
        glEnableVertexAttribArray(0);

        int transformLoc = glGetUniformLocation(shaderProgram_, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform.data());

        int colorLoc = glGetUniformLocation(shaderProgram_, "objectColor");
        glUniform3f(colorLoc, color.x(), color.y(), color.z());

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}