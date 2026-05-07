#ifndef RENDERABLEMESH_H
#define RENDERABLEMESH_H

#include <memory>
#include <vector>

#include "renderer/materials/material.h"
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
    RenderableMesh() : id_(-1) {}

    /**
     * @brief Constructs a Renderable with vertices and mesh groups.
     * @param id Unique identifier for the renderable.
     * @param vertices Shared pointer to a vector of vertices.
     * @param meshGroups Shared pointer to a vector of mesh groups.
     */
    RenderableMesh(uint64_t id, std::shared_ptr<std::vector<Vertex>> vertices,
                   std::shared_ptr<std::vector<MeshGroup>> meshGroups);

    ~RenderableMesh();

    RenderableMesh(const RenderableMesh &) = delete;
    RenderableMesh &operator=(const RenderableMesh &) = delete;

    /*
        !!!REMEMBER TO CHANGE UPDATE THIS WHENEVER ADDING NEW STUFF TO RENDERABLE!!!
    */

    /**
     * @brief Move constructor for Renderable.
     * @param other The Renderable to move from.
     */
    RenderableMesh(RenderableMesh &&other) noexcept
        : vbo_(other.vbo_), vao_(other.vao_), vertices_(std::move(other.vertices_)),
          meshGroups_(std::move(other.meshGroups_)), shaderProgram_(other.shaderProgram_),
          id_(other.id_), modelLocation_(other.modelLocation_),
          diffuseLocation_(other.diffuseLocation_), specularLocation_(other.specularLocation_),
          shininessLocation_(other.shininessLocation_), dissolveLocation_(other.dissolveLocation_) {
        other.vbo_ = -1;
        other.vao_ = -1;
        other.id_ = -1;
    }

    /**
     * @brief Move assignment operator for Renderable.
     * @param other The Renderable to move from.
     * @return Reference to this Renderable.
     */
    RenderableMesh &operator=(RenderableMesh &&other) noexcept {
        if (this != &other) {
            vbo_ = other.vbo_;
            vao_ = other.vao_;
            vertices_ = std::move(other.vertices_);
            meshGroups_ = std::move(other.meshGroups_);
            shaderProgram_ = other.shaderProgram_;
            id_ = other.id_;
            modelLocation_ = other.modelLocation_;
            diffuseLocation_ = other.diffuseLocation_;
            specularLocation_ = other.specularLocation_;
            shininessLocation_ = other.shininessLocation_;
            dissolveLocation_ = other.dissolveLocation_;

            other.vbo_ = -1;
            other.vao_ = -1;
            other.id_ = -1;
        }
        return *this;
    }

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
     * @brief Gets the unique identifier of the renderable.
     * @return Unique ID (== to NodeID).
     */
    uint64_t getId() const {
        return id_;
    }

    bool isValid() const {
        return id_ != (uint64_t)-1;
    }

    /**
     * @brief Sets the shader program to use for rendering.
     * @param shaderProgram Shader program handle.
     */
    void setShaderProgram(uint32_t shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

private:
    uint32_t vbo_ = -1;
    uint32_t vao_ = -1;

    std::shared_ptr<std::vector<Vertex>> vertices_;
    std::shared_ptr<std::vector<MeshGroup>> meshGroups_;
    uint32_t shaderProgram_ = 0;
    uint64_t id_ = -1;

    // Locations caching
    // begin
    int modelLocation_;

    int diffuseLocation_;
    int specularLocation_;
    int shininessLocation_;
    int dissolveLocation_;
    // end
};

#endif