#ifndef ENABLEABLE_H
#define ENABLEABLE_H

/**
 * @class Enableable
 * @brief A simple class to manage an enabled/disabled state.
 */
class Enableable {
public:
    Enableable() = default;

    void enable() {
        enabled_ = true;
    }

    void disable() {
        enabled_ = false;
    }

    bool isEnabled() {
        return enabled_;
    }

private:
    bool enabled_;
};

#endif