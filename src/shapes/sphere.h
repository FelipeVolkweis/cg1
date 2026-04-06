#ifndef SPHERE_H
#define SPHERE_H

#include "baseshape.h"

/**
 * @class Sphere
 * @brief Represents a 3D sphere shape, composed of a spherical surface approximated by triangles arranged in stacks and sectors.
 */
class Sphere : public BaseShape {
public:
    Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);

    std::string getName() const override {
        return "sphere";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float radius, int sectors, int stacks);
};

#endif