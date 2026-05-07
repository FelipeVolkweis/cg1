#ifndef VEHICLE_H
#define VEHICLE_H

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "math/transformations/transformation.h"
#include "physics/physicsengine.h"
#include "physics/rigidbody.h"

class BaseShape;

/**
 * @brief Class representing a vehicle in the physics simulation.
 */
class Vehicle {
public:
    /**
     * @brief Constructs a new Vehicle object.
     *
     * @param physicsEngine Pointer to the physics engine.
     * @param mass The mass of the vehicle's chassis.
     * @param initialTransform The initial transformation of the vehicle.
     * @param chassisShape The shape of the vehicle's chassis.
     */
    Vehicle(PhysicsEngine *physicsEngine, float mass, const Transformation &initialTransform,
            const BaseShape *chassisShape);
    ~Vehicle();

    void addToWorld(btDiscreteDynamicsWorld *world);
    void removeFromWorld(btDiscreteDynamicsWorld *world);

    /**
     * @brief Adds a wheel to the vehicle.
     *
     * @param connectionPointCS0 The connection point of the wheel in chassis space.
     * @param suspensionRestLength The rest length of the suspension.
     * @param wheelRadius The radius of the wheel.
     * @param isFrontWheel True if the wheel is a front wheel.
     */
    void addWheel(const btVector3 &connectionPointCS0, float suspensionRestLength,
                  float wheelRadius, bool isFrontWheel);

    /**
     * @brief Applies engine force to a specific wheel.
     *
     * @param force The force to apply.
     * @param wheelIndex The index of the wheel.
     */
    void applyEngineForce(float force, int wheelIndex);

    /**
     * @brief Sets the steering value for a specific wheel.
     *
     * @param steering The steering value.
     * @param wheelIndex The index of the wheel.
     */
    void setSteeringValue(float steering, int wheelIndex);

    /**
     * @brief Sets the brake force for a specific wheel.
     *
     * @param brake The brake force.
     * @param wheelIndex The index of the wheel.
     */
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
