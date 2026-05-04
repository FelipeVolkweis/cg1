#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <memory>
#include <vector>

#include "types/mat4x4.h"
#include "types/vertex.h"

class Node;

struct MeshGroup {
    uint32_t textureId;
    size_t start;
    size_t count;

    bool translucent;
};

class Renderable {
public:
    Renderable() : id_(-1) {}

    Renderable(uint64_t id, std::shared_ptr<std::vector<Vertex>> vertices,
               std::shared_ptr<std::vector<MeshGroup>> meshGroups);

    ~Renderable();

    Renderable(const Renderable &) = delete;
    Renderable &operator=(const Renderable &) = delete;

    Renderable(Renderable &&other) noexcept
        : vbo_(other.vbo_), vao_(other.vao_), vertices_(std::move(other.vertices_)),
          meshGroups_(std::move(other.meshGroups_)), shaderProgram_(other.shaderProgram_),
          id_(other.id_) {
        other.vbo_ = -1;
        other.vao_ = -1;
        other.id_ = -1;
    }

    Renderable &operator=(Renderable &&other) noexcept {
        if (this != &other) {
            vbo_ = other.vbo_;
            vao_ = other.vao_;
            vertices_ = std::move(other.vertices_);
            meshGroups_ = std::move(other.meshGroups_);
            shaderProgram_ = other.shaderProgram_;
            id_ = other.id_;

            other.vbo_ = -1;
            other.vao_ = -1;
            other.id_ = -1;
        }
        return *this;
    }

    void initializeOnGPU();
    void render(const Mat4x4 &model);

    uint32_t getVAO() const {
        return vao_;
    }

    uint32_t getVBO() const {
        return vbo_;
    }

    bool hasVAO() const {
        return vao_ != (uint32_t)-1;
    }

    bool hasVBO() const {
        return vbo_ != (uint32_t)-1;
    }

    uint64_t getId() const {
        return id_;
    }

    bool isValid() const {
        return id_ != (uint64_t)-1;
    }

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
};

#endif