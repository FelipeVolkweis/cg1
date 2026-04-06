#ifndef CONE_H
#define CONE_H

#include "baseshape.h"

/**
 * @class Cone
 * @brief Represents a 3D cone shape, composed of a circular base and a lateral surface, both
 * approximated by triangles.
 */
class Cone : public BaseShape {
public:
    Cone(float radius = 1.0f, float height = 2.0f, int sectors = 36);

    std::string getName() const override {
        return "cone";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float radius, float height, int sectors);
};

#endif