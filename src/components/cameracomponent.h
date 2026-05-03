#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <iosfwd>
#include <memory>

#include "camera/camera.h"
#include "core/component.h"
#include "core/enableable.h"

class CameraComponent : public BaseComponent, public Enableable {
public:
    CameraComponent() = default;
    CameraComponent(Perspective perspective, Vec3 focalPoint = Vec3(0, 0, 0),
                    Vec3 up = Vec3(0, 1, 0));

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    std::shared_ptr<Camera> getCamera() {
        return camera_;
    }

    void onUpdate(float dt) override;

private:
    std::shared_ptr<Camera> camera_;
    Vec3 focalPoint_;
    Vec3 up_;
};

#endif
