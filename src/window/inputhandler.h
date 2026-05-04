#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>

#include "types/vec2.h"

class InputHandler {
public:
    InputHandler(GLFWwindow *window);

    void handleWASD();
    void handleIJKL();
    void handleWireframe_P();
    void handleCameraCycle_C();
    void handleBrakes_SpaceBar();

    std::pair<int, int> getWindowDimensions() const;

    Vec2 getMainYAxisInputVector();
    Vec2 getMainXAxisInputVector();
    Vec2 getSecondaryYAxisInputVector();
    Vec2 getSecondaryXAxisInputVector();
    bool getWireframeEnabled();

    uint32_t getNumberOfCameraCycleRequests() const {
        return cycleClicked_;
    }

    bool isSpaceBarHeldDown() const {
        return spaceHeldDown_;
    }

    const int W_KEY = 0;
    const int A_KEY = 1;
    const int S_KEY = 2;
    const int D_KEY = 3;

    const int I_KEY = 0;
    const int J_KEY = 1;
    const int K_KEY = 2;
    const int L_KEY = 3;

private:
    int wasd_[4];
    int ijkl_[4];
    bool wireframe_;
    bool wireframeKeyPressed_;
    GLFWwindow *window_;

    uint32_t cycleClicked_ = 0;
    bool cycleKeyPressed_ = false;
    bool spaceHeldDown_ = false;
};

#endif