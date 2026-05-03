#ifndef BASESHAPE_H
#define BASESHAPE_H

#include <memory>
#include <string>
#include <vector>

#include "types/vec3.h"

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
    BaseShape() : color_(1.0f, 1.0f, 1.0f) {}

    virtual ~BaseShape() = default;

    virtual std::string getName() const = 0;
    virtual void parse(const YAML::Node &node) = 0;

    void parseCommon(const YAML::Node &node);

    const std::vector<Vec3> &getVertices() const {
        return vertices_;
    }

    void addVertex(const Vec3 &vertex) {
        vertices_.push_back(vertex);
    }

    const Vec3 &getColor() const {
        return color_;
    }

    void setColor(const Vec3 &color) {
        color_ = color;
    }

protected:
    std::vector<Vec3> vertices_;
    Vec3 color_;
};

#endif