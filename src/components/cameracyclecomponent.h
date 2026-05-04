#ifndef CAMERACYCLECOMPONENT_H
#define CAMERACYCLECOMPONENT_H

#include "core/component.h"
#include "window/inputhandler.h"

/**
 * @class CameraCycleComponent
 * @brief Component that cycles through available cameras based on input.
 */
class CameraCycleComponent : public BaseComponent {
public:
    CameraCycleComponent() = default;

    /**
     * @brief Constructor for CameraCycleComponent.
     * @param inputHandler Pointer to the input handler.
     */
    CameraCycleComponent(InputHandler *inputHandler);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    bool onStart() override;
    void onPreUpdate() override;

private:
    InputHandler *inputHandler_ = nullptr;
};

#endif