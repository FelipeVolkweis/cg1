#ifndef SEMISPHERE_H
#define SEMISPHERE_H

#include "baseshape.h"

/**
 * @class SemiSphere
 * @brief Represents a 3D semi-sphere shape, composed of a hemispherical surface approximated by
 * triangles.
 */
class SemiSphere : public BaseShape {
public:
    SemiSphere(float radius = 1.0f, int sectors = 36, int stacks = 18);

    std::string getName() const override {
        return "semisphere";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float radius, int sectors, int stacks);
};

#endif