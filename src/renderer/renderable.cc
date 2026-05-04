#include "renderable.h"

#include <glad/glad.h>

Renderable::Renderable(uint64_t id, std::shared_ptr<std::vector<Vertex>> vertices,
                       std::shared_ptr<std::vector<MeshGroup>> meshGroups)
    : vertices_(vertices), meshGroups_(meshGroups), id_(id) {}

void Renderable::initializeOnGPU() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_->size() * sizeof(Vertex), vertices_->data(),
                 GL_STATIC_DRAW);

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
}
void Renderable::render(const Mat4x4 &model, bool renderTranslucent){
    glBindVertexArray(vao_);

    int modelLoc = glGetUniformLocation(shaderProgram_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

    glActiveTexture(GL_TEXTURE0);
    int textureLoc = glGetUniformLocation(shaderProgram_, "texture_diffuse");
    glUniform1i(textureLoc, 0);

    for (const auto &mesh : *meshGroups_) { 
        if (renderTranslucent && mesh.translucent || 
            !renderTranslucent && !mesh.translucent) {
            if (mesh.translucent) {
                glDepthMask(GL_FALSE);
            } else {
                glDepthMask(GL_TRUE);
            }

            glBindTexture(GL_TEXTURE_2D, mesh.textureId);
            glDrawArrays(GL_TRIANGLES, mesh.start, mesh.count);
        }
    }
    
    glDepthMask(GL_TRUE);
}

Renderable::~Renderable() {
    if (vao_ != (uint32_t)-1)
        glDeleteVertexArrays(1, &vao_);
    if (vbo_ != (uint32_t)-1)
        glDeleteBuffers(1, &vbo_);
}
