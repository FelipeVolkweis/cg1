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
uniform vec3 objectColor;

void main() {
    vec4 texColor = texture(texture_diffuse, TexCoord);

    FragColor = texColor * vec4(objectColor, 1.0f) * VertColor;
}
)";

Renderer::~Renderer() {
    if (vao_)
        glDeleteVertexArrays(1, &vao_);
    if (vbo_)
        glDeleteBuffers(1, &vbo_);
    if (ebo_)
        glDeleteBuffers(1, &ebo_);
    if (shaderProgram_)
        glDeleteProgram(shaderProgram_);
    if (whiteTextureId_)
        glDeleteTextures(1, &whiteTextureId_);
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
    glGenBuffers(1, &ebo_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    whiteTextureId_ = Texture::createWhiteTexture();

    return true;
}

void Renderer::render() {
    if (auto activeCamera = activeCamera_.lock()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (skybox_) {
            skybox_->render(activeCamera->lookAt(), activeCamera->perspective());
        }

        glUseProgram(shaderProgram_);

        int viewLoc = glGetUniformLocation(shaderProgram_, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, activeCamera->lookAt().data());

        int projLoc = glGetUniformLocation(shaderProgram_, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, activeCamera->perspective().data());

        glActiveTexture(GL_TEXTURE0);
        int textureLoc = glGetUniformLocation(shaderProgram_, "texture_diffuse");
        glUniform1i(textureLoc, 0);

        glDepthMask(GL_TRUE);
        for (const RenderItem &item : items_) {
            if (!item.shape->hasColor() && !item.shape->isModel()) {
                continue;
            }

            const std::vector<Vertex> &vertices = item.shape->getVertices();
            if (vertices.empty())
                continue;

            setupRenderingStep(item);

            const auto &groups = item.shape->getGroups();
            if (groups.empty()) {
                uint32_t texToBind = item.shape->getTextureId();
                if (texToBind == 0) {
                    texToBind = whiteTextureId_;
                }
                glBindTexture(GL_TEXTURE_2D, texToBind);
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            } else {
                for (const auto &group : groups) {
                    if (group.isTransparent)
                        continue;
                    uint32_t texToBind = group.textureId;
                    if (texToBind == 0) {
                        texToBind = whiteTextureId_;
                    }
                    glBindTexture(GL_TEXTURE_2D, texToBind);
                    glDrawArrays(GL_TRIANGLES, group.start, group.count);
                }
            }
        }

        for (const RenderItem &item : items_) {
            const auto &groups = item.shape->getGroups();
            bool hasTransparent = false;
            for (const auto &g : groups) {
                if (g.isTransparent) {
                    hasTransparent = true;
                    break;
                }
            }
            if (!hasTransparent)
                continue;

            setupRenderingStep(item);

            for (const auto &group : groups) {
                if (!group.isTransparent)
                    continue;
                uint32_t texToBind = group.textureId;
                if (texToBind == 0) {
                    texToBind = whiteTextureId_;
                }
                glBindTexture(GL_TEXTURE_2D, texToBind);
                glDrawArrays(GL_TRIANGLES, group.start, group.count);
            }
        }
    }
}

void Renderer::setupRenderingStep(const RenderItem &item) {
    const std::vector<Vertex> &vertices = item.shape->getVertices();
    const Mat4x4 &model = item.transform.getTransformationMatrix();
    const Vec3 &color = item.shape->getColor();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    int modelLoc = glGetUniformLocation(shaderProgram_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

    int colorLoc = glGetUniformLocation(shaderProgram_, "objectColor");
    glUniform3f(colorLoc, color.x(), color.y(), color.z());
}