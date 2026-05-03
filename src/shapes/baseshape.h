#ifndef BASESHAPE_H
#define BASESHAPE_H

#include <memory>
#include <string>
#include <vector>

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
    BaseShape() : color_(1.0f, 1.0f, 1.0f), textureId_(0) {}

    virtual ~BaseShape() = default;

    virtual std::string getName() const = 0;
    virtual void parse(const YAML::Node &node) = 0;

    void parseCommon(const YAML::Node &node);

    const std::vector<Vertex> &getVertices() const {
        return vertices_;
    }

    void addVertex(const Vertex &vertex) {
        vertices_.push_back(vertex);
    }

    const Vec3 &getColor() const {
        return color_;
    }

    void setColor(const Vec3 &color) {
        color_ = color;
    }

    uint32_t getTextureId() const {
        return textureId_;
    }

    void setTextureId(uint32_t id) {
        textureId_ = id;
    }

protected:
    std::vector<Vertex> vertices_;
    Vec3 color_;
    uint32_t textureId_;
};

#endif