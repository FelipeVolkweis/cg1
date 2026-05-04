#ifndef BASESHAPE_H
#define BASESHAPE_H

#include <memory>
#include <string>
#include <vector>

#include "renderer/renderable.h"
#include "types/vec3.h"
#include "types/vertex.h"

namespace YAML {
class Node;
}

/**
 * @class BaseShape
 * @brief Abstract base class for all 3D shapes, providing common attributes like color and
 * transformations.
 */
class BaseShape {
public:
    BaseShape() : color_(1.0f, 1.0f, 1.0f), textureId_(-1) {}

    virtual ~BaseShape() = default;

    virtual std::string getName() const = 0;

    /**
     * @brief Parses shape-specific data from a YAML node.
     * @param node The YAML node containing shape data.
     */
    virtual void parse(const YAML::Node &node) = 0;

    /**
     * @brief Parses common shape data from a YAML node (e.g., color, texture).
     * @param node The YAML node containing shape data.
     */
    void parseCommon(const YAML::Node &node);

    const std::vector<Vertex> &getVertices() const {
        return *vertices_;
    }

    void addVertex(const Vertex &vertex) {
        vertices_->push_back(vertex);
    }

    const Vec3 &getColor() const {
        return color_;
    }

    void setColor(const Vec3 &color) {
        color_ = color;
    }

    bool hasColor() const {
        return hasColor_;
    }

    bool isModel() const {
        return isModel_;
    }

    /**
     * @brief Converts the shape to a Renderable object.
     * @param id The unique ID (from Node) for the Renderable.
     * @return The Renderable object.
     */
    Renderable asRenderable(uint64_t id) const {
        return Renderable(id, vertices_, meshGroups_);
    }

protected:
    std::shared_ptr<std::vector<Vertex>> vertices_;
    Vec3 color_;
    uint32_t textureId_;
    std::shared_ptr<std::vector<MeshGroup>> meshGroups_;
    bool hasColor_ = false;
    bool isModel_ = false;

    void initializeCommon();
};

#endif