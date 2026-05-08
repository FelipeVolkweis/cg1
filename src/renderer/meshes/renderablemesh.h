#ifndef RENDERABLEMESH_H
#define RENDERABLEMESH_H

#include <memory>
#include <vector>

#include "renderer/materials/material.h"
#include "renderer/shader/shader.h"
#include "types/mat4x4.h"
#include "types/vertex.h"

class Node;

/**
 * @struct MeshGroup
 * @brief Represents a group of meshes with the same texture and translucency.
 */
struct MeshGroup {
    Material material;
    size_t start;
    size_t count;

    bool translucent;
};

/**
 * @class Renderable
 * @brief Represents an object that can be rendered on the GPU.
 */
class RenderableMesh {
public:
    /**
     * @brief Constructs a Renderable with vertices and mesh groups.
     * @param id Unique identifier for the renderable.
     * @param vertices Shared pointer to a vector of vertices.
     * @param meshGroups Shared pointer to a vector of mesh groups.
     */
    RenderableMesh(std::shared_ptr<std::vector<Vertex>> vertices,
                   std::shared_ptr<std::vector<MeshGroup>> meshGroups);

    ~RenderableMesh();

    /**
     * @brief Initializes the renderable on the GPU (creates VAO/VBO).
     */
    void initializeOnGPU();

    /**
     * @brief Renders the object using the provided model matrix.
     * @param model The model transformation matrix.
     * @param renderTranslucent Whether to render translucent parts if true
     * or render opaque if false.
     */
    void render(const Mat4x4 &model, bool renderTranslucent);

    /**
     * @brief Renders the object for the shadow pass.
     * Assumes the correct shadow shader is already active.
     */
    void renderShadow();

    /**
     * @brief Gets the Vertex Array Object (VAO) handle.
     * @return VAO handle.
     */
    uint32_t getVAO() const {
        return vao_;
    }

    /**
     * @brief Gets the Vertex Buffer Object (VBO) handle.
     * @return VBO handle.
     */
    uint32_t getVBO() const {
        return vbo_;
    }

    bool hasVAO() const {
        return vao_ != (uint32_t)-1;
    }

    bool hasVBO() const {
        return vbo_ != (uint32_t)-1;
    }

    /**
     * @brief Sets the shader program to use for rendering.
     * @param shaderProgram Shader program handle.
     */
    void setShaderProgram(std::shared_ptr<Shader> shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

private:
    uint32_t vbo_ = -1;
    uint32_t vao_ = -1;

    std::shared_ptr<std::vector<Vertex>> vertices_;
    std::shared_ptr<std::vector<MeshGroup>> meshGroups_;
    std::shared_ptr<Shader> shaderProgram_;
};

#endif