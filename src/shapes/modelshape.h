#ifndef MODELSHAPE_H
#define MODELSHAPE_H

#include <string>

#include "baseshape.h"

/**
 * @class ModelShape
 * @brief Represents a shape loaded from a 3D model file.
 */
class ModelShape : public BaseShape {
public:
    ModelShape() {
        isModel_ = true;
    }

    std::string getName() const override {
        return "model";
    }

    void parse(const YAML::Node &node) override;
};

#endif