#include "componentfactory.h"

#include "components/cameracomponent.h"
#include "components/cameracontrollercomponent.h"
#include "components/cameracyclecomponent.h"
#include "components/carcontrollercomponent.h"
#include "components/meshcomponent.h"
#include "components/orbitalcameracontrollercomponent.h"
#include "components/rigidbodycomponent.h"
#include "components/vehiclecomponent.h"

ComponentFactory::ComponentFactory() {
    registerComponent("Camera", []() { return std::make_shared<CameraComponent>(); });
    registerComponent("Mesh", []() { return std::make_shared<MeshComponent>(); });
    registerComponent("CarController", []() { return std::make_shared<CarControllerComponent>(); });
    registerComponent("RigidBody", []() { return std::make_shared<RigidBodyComponent>(); });
    registerComponent("Vehicle", []() { return std::make_shared<VehicleComponent>(); });
    registerComponent("CameraController",
                      []() { return std::make_shared<CameraControllerComponent>(); });
    registerComponent("OrbitalCameraController",
                      []() { return std::make_shared<OrbitalCameraControllerComponent>(); });
    registerComponent("CameraCycle", []() { return std::make_shared<CameraCycleComponent>(); });
}

ComponentFactory &ComponentFactory::getInstance() {
    static ComponentFactory instance;
    return instance;
}

void ComponentFactory::registerComponent(const std::string &type, Creator creator) {
    creators_[type] = creator;
}

std::shared_ptr<BaseComponent> ComponentFactory::createComponent(const std::string &type) {
    auto it = creators_.find(type);
    if (it != creators_.end()) {
        return it->second();
    }
    return nullptr;
}
