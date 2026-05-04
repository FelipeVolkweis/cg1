#ifndef STEERINGANIMATIONCOMPONENT_H
#define STEERINGANIMATIONCOMPONENT_H

#include <string>

#include "core/component.h"
#include "transformations/transformation.h"

class SteeringAnimationComponent : public BaseComponent {
public:
    SteeringAnimationComponent() = default;

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    bool onStart() override;
    void onUpdate(float dt) override;

private:
    Transformation startingTransformation_;
    float maxRotation_ = 360.0f;
    std::string steeringNodeName_ = "SteeringVisuals";
};

#endif