#ifndef TORUS_H
#define TORUS_H

#include "baseshape.h"

class Torus : public BaseShape {
public:
    Torus(float innerRadius = 0.5f, float outerRadius = 1.0f, int sides = 36, int rings = 36);

    std::string getName() const override {
        return "torus";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float innerRadius, float outerRadius, int sides, int rings);
};

#endif