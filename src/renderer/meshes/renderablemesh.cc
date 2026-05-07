#include "renderablemesh.h"

#include <glad/glad.h>

RenderableMesh::RenderableMesh(std::shared_ptr<std::vector<Vertex>> vertices,
                               std::shared_ptr<std::vector<MeshGroup>> meshGroups)
    : vertices_(vertices), meshGroups_(meshGroups) {}

void RenderableMesh::initializeOnGPU() {
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
}

void RenderableMesh::render(const Mat4x4 &model, bool renderTranslucent) {
    glBindVertexArray(vao_);

    shaderProgram_->setMat4x4("model", model);
    shaderProgram_->setInt("material.diffuse", 0);
    shaderProgram_->setInt("material.specular", 1);

    for (const auto &mesh : *meshGroups_) {
        if (renderTranslucent && mesh.translucent || !renderTranslucent && !mesh.translucent) {
            if (mesh.translucent) {
                glDepthMask(GL_FALSE);
            } else {
                glDepthMask(GL_TRUE);
            }
            shaderProgram_->setFloat("material.shininess", mesh.material.getShininess());
            shaderProgram_->setFloat("material.dissolve", mesh.material.getDissolve());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.material.getDiffuse());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mesh.material.getSpecular());
            glDrawArrays(GL_TRIANGLES, mesh.start, mesh.count);
        }
    }

    glDepthMask(GL_TRUE);
}

RenderableMesh::~RenderableMesh() {
    if (vao_ != (uint32_t)-1)
        glDeleteVertexArrays(1, &vao_);
    if (vbo_ != (uint32_t)-1)
        glDeleteBuffers(1, &vbo_);
}
