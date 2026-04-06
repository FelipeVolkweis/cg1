#ifndef CYLINDER_H
#define CYLINDER_H

#include "baseshape.h"

class Cylinder : public BaseShape {
public:
    Cylinder(float radius = 1.0f, float height = 2.0f, int sectors = 36);

    std::string getName() const override {
        return "cylinder";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float radius, float height, int sectors);
};

#endif