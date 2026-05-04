#include "rigidbody.h"

#include <stdexcept>

#include "shapes/baseshape.h"

RigidBody::RigidBody(float mass, const Transformation &initialTransform, const BaseShape *shape) {
    auto hull = std::make_unique<btConvexHullShape>();
    for (const auto &v : shape->getVertices()) {
        hull->addPoint(btVector3(v.position.x(), v.position.y(), v.position.z()), false);
    }
    hull->recalcLocalAabb();
    shape_ = std::move(hull);

    const Mat4x4 &mat = initialTransform.getTransformationMatrix();

    float scaleX = mat.block<3, 1>(0, 0).norm();
    float scaleY = mat.block<3, 1>(0, 1).norm();
    float scaleZ = mat.block<3, 1>(0, 2).norm();

    shape_->setLocalScaling(btVector3(scaleX, scaleY, scaleZ));

    Mat4x4 normalizedMat = mat;
    if (scaleX > 0) normalizedMat.block<3, 1>(0, 0) /= scaleX;
    if (scaleY > 0) normalizedMat.block<3, 1>(0, 1) /= scaleY;
    if (scaleZ > 0) normalizedMat.block<3, 1>(0, 2) /= scaleZ;

    btTransform bulletTransform;
    bulletTransform.setFromOpenGLMatrix(normalizedMat.data());

    motionState_ = std::make_unique<btDefaultMotionState>(bulletTransform);

    btVector3 localInertia(0, 0, 0);
    if (mass != 0.0f) {
        shape_->calculateLocalInertia(mass, localInertia);
    }

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState_.get(), shape_.get(),
                                                    localInertia);
    body_ = std::make_unique<btRigidBody>(rbInfo);
}

RigidBody::~RigidBody() = default;

Transformation RigidBody::getTransform() const {
    btTransform bulletTransform;
    if (body_->getMotionState()) {
        body_->getMotionState()->getWorldTransform(bulletTransform);
    } else {
        bulletTransform = body_->getWorldTransform();
    }

    float m[16];
    bulletTransform.getOpenGLMatrix(m);

    Mat4x4 eigenMat;
    eigenMat << m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3],
        m[7], m[11], m[15];

    btVector3 scale = shape_->getLocalScaling();
    eigenMat.block<3, 1>(0, 0) *= scale.x();
    eigenMat.block<3, 1>(0, 1) *= scale.y();
    eigenMat.block<3, 1>(0, 2) *= scale.z();

    return Transformation(eigenMat);
}

void RigidBody::setTransform(const Transformation &transform) {
    const Mat4x4 &mat = transform.getTransformationMatrix();

    float scaleX = mat.block<3, 1>(0, 0).norm();
    float scaleY = mat.block<3, 1>(0, 1).norm();
    float scaleZ = mat.block<3, 1>(0, 2).norm();

    shape_->setLocalScaling(btVector3(scaleX, scaleY, scaleZ));

    Mat4x4 normalizedMat = mat;
    if (scaleX > 0) normalizedMat.block<3, 1>(0, 0) /= scaleX;
    if (scaleY > 0) normalizedMat.block<3, 1>(0, 1) /= scaleY;
    if (scaleZ > 0) normalizedMat.block<3, 1>(0, 2) /= scaleZ;

    btTransform bulletTransform;
    bulletTransform.setFromOpenGLMatrix(normalizedMat.data());

    body_->setWorldTransform(bulletTransform);
    if (body_->getMotionState()) {
        body_->getMotionState()->setWorldTransform(bulletTransform);
    }
}
