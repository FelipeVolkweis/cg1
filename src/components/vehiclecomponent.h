#ifndef VEHICLECOMPONENT_H
#define VEHICLECOMPONENT_H

#include "core/component.h"
#include "physics/physicsengine.h"
#include "physics/vehicle/vehicle.h"

class VehicleComponent : public BaseComponent {
public:
    VehicleComponent(PhysicsEngine *physicsEngine, float mass,
                     std::unique_ptr<btCollisionShape> shape);

    bool onStart() override;
    void onUpdate(float dt) override;
    bool onEnd() override;

    Vehicle *getVehicle() const {
        return vehicle_.get();
    }

    void addWheel(std::shared_ptr<Node> wheelNode, const btVector3 &connectionPointCS0,
                  float suspensionRestLength, float wheelRadius, bool isFrontWheel);

private:
    PhysicsEngine *physicsEngine_;
    std::unique_ptr<Vehicle> vehicle_;

    float initialMass_;
    std::unique_ptr<btCollisionShape> initialShape_;

    struct WheelData {
        std::shared_ptr<Node> node;
        btVector3 connectionPoint;
        float suspensionRestLength;
        float radius;
        bool isFrontWheel;
    };

    std::vector<WheelData> wheelsToInitialize_;
    std::vector<std::shared_ptr<Node>> activeWheelNodes_;
};

#endif
