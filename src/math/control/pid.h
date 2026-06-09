#ifndef PID_HH
#define PID_HH

class PID {
public:
    PID(float kp, float ki, float kd, float max);
    PID() {};
    float computeSignal(float error, float dt);
    void reset();
    void setConstants(float kp, float ki, float kd, float max);

private:
    float kp_;
    float ki_;
    float kd_;
    float max_;

    float integral_;

    float lastError_;

    bool firstRun_ = true;
};

#endif