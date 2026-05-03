#include "skybox.h"

#include <glad/glad.h>

#include "texture.h"
#include "utils/logger.h"

const char *skyboxVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
)";

const char *skyboxFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {    
    FragColor = texture(skybox, TexCoords);
}
)";

Skybox::Skybox(const std::vector<std::string> &faces) {
    textureId_ = Texture::loadCubemap(faces);
    setupGeometry();
    setupShaders();
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
    glDeleteProgram(shaderProgram_);
}

void Skybox::render(const Mat4x4 &view, const Mat4x4 &projection) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shaderProgram_);

    Mat4x4 skyView = view;
    skyView.data()[12] = 0;
    skyView.data()[13] = 0;
    skyView.data()[14] = 0;

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, "view"), 1, GL_FALSE, skyView.data());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, "projection"), 1, GL_FALSE,
                       projection.data());

    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

uint32_t Skybox::compileShader(uint32_t type, const char *source) {
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::Error("Error in skybox shader compilation", infoLog);
    }
    return shader;
}

void Skybox::setupGeometry() {
    float skyboxVertices[] = {                     //   Coordinates
                              -1.0f, -1.0f, 1.0f,  //        7--------6
                              1.0f,  -1.0f, 1.0f,  //       /|       /|
                              1.0f,  -1.0f, -1.0f, //      4--------5 |
                              -1.0f, -1.0f, -1.0f, //      | |      | |
                              -1.0f, 1.0f,  1.0f,  //      | 3------|-2
                              1.0f,  1.0f,  1.0f,  //      |/       |/
                              1.0f,  1.0f,  -1.0f, //      0--------1
                              -1.0f, 1.0f,  -1.0f};

    unsigned int skyboxIndices[] = {// Right
                                    1, 2, 6, 6, 5, 1,
                                    // Left
                                    0, 4, 7, 7, 3, 0,
                                    // Top
                                    4, 5, 6, 6, 7, 4,
                                    // Bottom
                                    0, 3, 2, 2, 1, 0,
                                    // Back
                                    0, 1, 5, 5, 4, 0,
                                    // Front
                                    3, 7, 6, 6, 2, 3};

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void Skybox::setupShaders() {
    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, skyboxVertexShaderSource);
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, skyboxFragmentShaderSource);

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        Logger::Error("Error in skybox shader program linkage", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
