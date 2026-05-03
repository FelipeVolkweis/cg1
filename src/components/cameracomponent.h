#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <iosfwd>
#include <memory>

#include "camera/camera.h"
#include "core/component.h"

class CameraComponent : public BaseComponent {
public:
    CameraComponent(Perspective perspective, Vec3 focalPoint = Vec3(0, 0, 0),
                    Vec3 up = Vec3(0, 1, 0));

    static std::shared_ptr<CameraComponent> parse(std::istream &iss, Vec3 &outPosition);

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
