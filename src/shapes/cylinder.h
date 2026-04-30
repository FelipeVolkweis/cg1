#ifndef CYLINDER_H
#define CYLINDER_H

#include "baseshape.h"

/**
 * @class Cylinder
 * @brief Represents a 3D cylinder shape, composed of two circular bases and a lateral surface, all
 * approximated by triangles.
 */
class Cylinder : public BaseShape {
public:
    Cylinder(float radius = 1.0f, float height = 2.0f, int sectors = 36);

    std::string getName() const override {
        return "cylinder";
    }

    Transformation parse(std::istream &is) override;

private:
    void generateVertices(float radius, float height, int sectors);
};

#endif