#ifndef VEHICLE_H
#define VEHICLE_H

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "physics/physicsengine.h"
#include "physics/rigidbody.h"
#include "transformations/transformation.h"

class Vehicle {
public:
    Vehicle(PhysicsEngine *physicsEngine, float mass, const Transformation &initialTransform,
            std::unique_ptr<btCollisionShape> chassisShape);
    ~Vehicle();

    void addToWorld(btDiscreteDynamicsWorld *world);
    void removeFromWorld(btDiscreteDynamicsWorld *world);

    void addWheel(const btVector3 &connectionPointCS0, float suspensionRestLength,
                  float wheelRadius, bool isFrontWheel);

    void applyEngineForce(float force, int wheelIndex);
    void setSteeringValue(float steering, int wheelIndex);
    void setBrake(float brake, int wheelIndex);

    Transformation getChassisTransform() const;
    Transformation getWheelTransform(int wheelIndex) const;

    btRaycastVehicle *getBulletVehicle() const {
        return vehicle_.get();
    }

    RigidBody *getChassis() const {
        return chassis_.get();
    }

private:
    std::unique_ptr<RigidBody> chassis_;
    std::unique_ptr<btVehicleRaycaster> vehicleRaycaster_;
    std::unique_ptr<btRaycastVehicle::btVehicleTuning> tuning_;
    std::unique_ptr<btRaycastVehicle> vehicle_;
};

#endif
