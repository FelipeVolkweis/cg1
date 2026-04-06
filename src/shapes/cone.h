#ifndef CONE_H
#define CONE_H

#include "baseshape.h"

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