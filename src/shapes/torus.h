#ifndef TORUS_H
#define TORUS_H

#include "baseshape.h"

/**
 * @class Torus
 * @brief Represents a 3D torus shape, composed of a doughnut-shaped surface approximated by
 * triangles arranged in rings and sides.
 */
class Torus : public BaseShape {
public:
    Torus(float innerRadius = 0.5f, float outerRadius = 1.0f, int sides = 36, int rings = 36);

    std::string getName() const override {
        return "torus";
    }

    Transformation parse(std::istream &is) override;

private:
    void generateVertices(float innerRadius, float outerRadius, int sides, int rings);
};

#endif