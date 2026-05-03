#ifndef MODELSHAPE_H
#define MODELSHAPE_H

#include <string>

#include "baseshape.h"

class ModelShape : public BaseShape {
public:
    ModelShape() = default;

    std::string getName() const override {
        return "model";
    }

    void parse(const YAML::Node &node) override;
};

#endif