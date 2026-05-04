#ifndef VEHICLECOMPONENT_H
#define VEHICLECOMPONENT_H

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "core/component.h"
#include "physics/physicsengine.h"
#include "physics/vehicle/vehicle.h"
#include "transformations/transformation.h"

class VehicleComponent : public BaseComponent {
public:
    VehicleComponent() = default;
    VehicleComponent(PhysicsEngine *physicsEngine, float mass);

    void load(const YAML::Node &data, PhysicsEngine &physicsEngine,
              InputHandler &inputHandler) override;

    bool onStart() override;
    void onUpdate(float dt) override;
    bool onEnd() override;

    Vehicle *getVehicle() const {
        return vehicle_.get();
    }

    void addWheel(std::shared_ptr<Node> wheelNode, const btVector3 &connectionPointCS0,
                  float suspensionRestLength, float wheelRadius, bool isFrontWheel);

private:
    PhysicsEngine *physicsEngine_ = nullptr;
    std::unique_ptr<Vehicle> vehicle_;

    float initialMass_ = 0.0f;

    struct WheelData {
        std::shared_ptr<Node> node;
        btVector3 connectionPoint;
        float suspensionRestLength;
        float radius;
        bool isFrontWheel;
    };

    struct ParsedWheelData {
        std::string nodeName;
        btVector3 connectionPoint;
        float suspensionRestLength;
        float radius;
        bool isFrontWheel;
    };

    struct WheelNode {
        std::shared_ptr<Node> node;
        Transformation offset;
    };

    std::vector<WheelData> wheelsToInitialize_;
    std::vector<ParsedWheelData> parsedWheels_;
    std::vector<WheelNode> activeWheelNodes_;
};

#endif
