#ifndef BOX_H
#define BOX_H

#include "baseshape.h"

class Box : public BaseShape {
public:
    Box(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

    std::string getName() const override {
        return "box";
    }

    void parse(std::istream &is) override;

private:
    void generateVertices(float width, float height, float depth);
};

#endif