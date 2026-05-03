#include "cameracyclecomponent.h"

#include "components/cameracomponent.h"
#include "components/cameracontrollercomponent.h"
#include "components/orbitalcameracontrollercomponent.h"
#include "core/node.h"

CameraCycleComponent::CameraCycleComponent(InputHandler *inputHandler)
    : inputHandler_(inputHandler) {}

bool CameraCycleComponent::onStart() {
    auto node = getNode();
    if (!inputHandler_ || !node)
        return false;

    auto outer = Node::findNodeByName(node, "OuterCameraAnchor");
    auto inner = Node::findNodeByName(node, "InnerCameraAnchor");

    if (!outer || !inner)
        return false;

    auto orbitalControl = outer->getComponent<OrbitalCameraControllerComponent>();
    auto cameraControl = inner->getComponent<CameraControllerComponent>();

    auto outerCamera = outer->getComponent<CameraComponent>();
    auto innerCamera = inner->getComponent<CameraComponent>();

    if (!orbitalControl || !cameraControl || !outerCamera || !innerCamera)
        return false;

    orbitalControl->enable();
    outerCamera->enable();
    cameraControl->disable();
    innerCamera->disable();

    return true;
}

void CameraCycleComponent::onPreUpdate() {
    auto node = getNode();
    if (!inputHandler_ || !node)
        return;

    auto outer = Node::findNodeByName(node, "OuterCameraAnchor");
    auto inner = Node::findNodeByName(node, "InnerCameraAnchor");

    if (!outer || !inner)
        return;

    auto orbitalControl = outer->getComponent<OrbitalCameraControllerComponent>();
    auto cameraControl = inner->getComponent<CameraControllerComponent>();

    auto outerCamera = outer->getComponent<CameraComponent>();
    auto innerCamera = inner->getComponent<CameraComponent>();

    if (!orbitalControl || !cameraControl || !outerCamera || !innerCamera)
        return;

    auto cycles = inputHandler_->getNumberOfCameraCycleRequests();
    if (cycles % 2 == 0) {
        orbitalControl->enable();
        outerCamera->enable();
        cameraControl->disable();
        innerCamera->disable();
    } else {
        orbitalControl->disable();
        outerCamera->disable();
        cameraControl->enable();
        innerCamera->enable();
    }
}

void CameraCycleComponent::load(const YAML::Node &data, PhysicsEngine &physicsEngine,
                                InputHandler &inputHandler) {
    inputHandler_ = &inputHandler;
}