#include "pid.h"

#include <algorithm>

PID::PID(float kp, float ki, float kd, float max)
    : kp_(kp), ki_(ki), kd_(kd), max_(max), lastError_(0), integral_(0) {}

float PID::computeSignal(float error, float dt) {
    integral_ = std::clamp(integral_ + (error * dt), -max_, max_);

    float p = kp_ * error;
    float i = ki_ * integral_;
    float d = kd_ * (error - lastError_) / dt;

    if (firstRun_) {
        d = 0;
        firstRun_ = false;
    }

    lastError_ = error;

    float signal = p + i + d;

    return signal;
}

void PID::reset() {
    lastError_ = 0;
    integral_ = 0;
}

void PID::setConstants(float kp, float ki, float kd, float max) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    max_ = max;
}