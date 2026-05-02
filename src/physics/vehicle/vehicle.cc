#include "vehicle.h"

Vehicle::Vehicle(PhysicsEngine *physicsEngine, float mass, const Transformation &initialTransform,
                 std::unique_ptr<btCollisionShape> chassisShape) {
    chassis_ = std::make_unique<RigidBody>(mass, initialTransform, std::move(chassisShape));
    chassis_->getBulletRigidBody()->setActivationState(DISABLE_DEACTIVATION);

    vehicleRaycaster_ = std::make_unique<btDefaultVehicleRaycaster>(physicsEngine->getWorld());

    tuning_ = std::make_unique<btRaycastVehicle::btVehicleTuning>();
    vehicle_ = std::make_unique<btRaycastVehicle>(*tuning_, chassis_->getBulletRigidBody(),
                                                  vehicleRaycaster_.get());

    // X is right (0), Y is up (1), Z is forward (2)
    vehicle_->setCoordinateSystem(0, 1, 2);
}

Vehicle::~Vehicle() = default;

void Vehicle::addToWorld(btDiscreteDynamicsWorld *world) {
    if (world) {
        world->addRigidBody(chassis_->getBulletRigidBody());
        world->addVehicle(vehicle_.get());
    }
}

void Vehicle::removeFromWorld(btDiscreteDynamicsWorld *world) {
    if (world) {
        world->removeVehicle(vehicle_.get());
        world->removeRigidBody(chassis_->getBulletRigidBody());
    }
}

void Vehicle::addWheel(const btVector3 &connectionPointCS0, float suspensionRestLength,
                       float wheelRadius, bool isFrontWheel) {
    btVector3 wheelDirectionCS0(0, -1, 0);
    btVector3 wheelAxleCS(-1, 0, 0);

    btWheelInfo &wheel =
        vehicle_->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength,
                           wheelRadius, *tuning_, isFrontWheel);

    wheel.m_suspensionStiffness = 20.0f;
    wheel.m_wheelsDampingRelaxation = 2.3f;
    wheel.m_wheelsDampingCompression = 4.4f;
    wheel.m_frictionSlip = 1000.0f;
    wheel.m_rollInfluence = 0.1f;
}

void Vehicle::applyEngineForce(float force, int wheelIndex) {
    vehicle_->applyEngineForce(force, wheelIndex);
}

void Vehicle::setSteeringValue(float steering, int wheelIndex) {
    vehicle_->setSteeringValue(steering, wheelIndex);
}

void Vehicle::setBrake(float brake, int wheelIndex) {
    vehicle_->setBrake(brake, wheelIndex);
}

Transformation Vehicle::getChassisTransform() const {
    return chassis_->getTransform();
}

Transformation Vehicle::getWheelTransform(int wheelIndex) const {
    vehicle_->updateWheelTransform(wheelIndex, true);
    const btTransform &bulletTransform = vehicle_->getWheelInfo(wheelIndex).m_worldTransform;

    float m[16];
    bulletTransform.getOpenGLMatrix(m);

    Mat4x4 eigenMat;
    eigenMat << m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3],
        m[7], m[11], m[15];

    return Transformation(eigenMat);
}
