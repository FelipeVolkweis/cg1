#ifndef CAMERACYCLECOMPONENT_H
#define CAMERACYCLECOMPONENT_H

#include "core/component.h"

/**
 * @class CameraCycleComponent
 * @brief Component that cycles through available cameras based on input.
 */
class CameraCycleComponent : public BaseComponent {
public:
    CameraCycleComponent() = default;

    void load(const YAML::Node &data) override;

    bool onStart() override;
    void onPreUpdate() override;
};

#endif