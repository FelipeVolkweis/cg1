#ifndef BASESHAPE_H
#define BASESHAPE_H

#include "transformations/transformation.h"
#include "types/vec3.h"
#include <istream>
#include <memory>
#include <string>
#include <vector>

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
    virtual void parse(std::istream &is) = 0;

    void parseCommon(std::istream &is);

    const std::vector<Vec3> &getVertices() const {
        return vertices_;
    }

    void addVertex(const Vec3 &vertex) {
        vertices_.push_back(vertex);
    }

    const Transformation &getTransformation() const {
        return transformation_;
    }

    void setTransformation(const Transformation &transformation) {
        transformation_ = transformation;
    }

    const Vec3 &getColor() const {
        return color_;
    }

    void setColor(const Vec3 &color) {
        color_ = color;
    }

    const std::vector<std::string> &getModifiers() const {
        return modifiers_;
    }

protected:
    std::vector<Vec3> vertices_;
    Transformation transformation_;
    Vec3 color_;
    std::vector<std::string> modifiers_;
};

#endif