#include "camera.h"

#include <cmath>

#include "math/projections/projection.h"
#include "utils/deg2rad.h"
#include "utils/logger.h"

Camera::Camera(Perspective perspective, LookAt lookAt)
    : fovy_(perspective.fovy), aspect_(perspective.aspect), zNear_(perspective.zNear),
      zFar_(perspective.zFar), position_(lookAt.position), focalPoint_(lookAt.focalPoint),
      up_(lookAt.up) {}

Mat4x4 Camera::lookAt() {
    return Projection::getLookAt(position_, focalPoint_, up_);
}

Mat4x4 Camera::perspective() {
    return Projection::getPerspective(fovy_, aspect_, zNear_, zFar_);
}
