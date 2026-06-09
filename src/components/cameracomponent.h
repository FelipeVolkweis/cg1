#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <iosfwd>
#include <memory>

#include "camera/camera.h"
#include "core/component.h"
#include "core/enableable.h"
#include "types/quaternion.h"

/**
 * @class CameraComponent
 * @brief Component that provides camera functionality to a Node.
 */
class CameraComponent : public BaseComponent, public Enableable {
public:
    CameraComponent() = default;

    /**
     * @brief Constructor for CameraComponent.
     * @param perspective Perspective settings for the camera.
     * @param focalPoint The point the camera is looking at.
     * @param up The up vector for the camera.
     */
    CameraComponent(Perspective perspective, Vec3 focalPoint = Vec3(0, 0, 0),
                    Vec3 up = Vec3(0, 1, 0));

    void load(const YAML::Node &data) override;

    std::shared_ptr<Camera> getCamera() {
        return camera_;
    }

    void onUpdate(float dt) override;

private:
    std::shared_ptr<Camera> camera_;
    Vec3 focalPoint_;
    Vec3 up_;

    float smoothTime_;
    float maxSpeed_;
    float rotationLambda_;

    Quaternion currentRotation_;
    Vec3 currentPosition_;
    Vec3 currentVelocity_;

    Vec3 smoothDamp(Vec3 currentPosition, Vec3 targetPosition, Vec3 &currentVelocity,
                    float smoothTime, float maxSpeed, float deltaTime);
};

#endif
