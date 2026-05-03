#ifndef CAMERACYCLECOMPONENT_H
#define CAMERACYCLECOMPONENT_H

#include "core/component.h"
#include "window/inputhandler.h"

class CameraCycleComponent : public BaseComponent {
public:
    CameraCycleComponent() = default;
    CameraCycleComponent(InputHandler *inputHandler);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    bool onStart() override;
    void onPreUpdate() override;

private:
    InputHandler *inputHandler_ = nullptr;
};

#endif