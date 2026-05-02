#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>

#include "types/vec2.h"

class InputHandler {
public:
    InputHandler(GLFWwindow *window);

    void handleWASD();
    void handleWireframe_P();

    Vec2 getYAxisInputVector();
    Vec2 getXAxisInputVector();
    bool getWireframeEnabled();

    const int W_KEY = 0;
    const int A_KEY = 1;
    const int S_KEY = 2;
    const int D_KEY = 3;

private:
    int wasd_[4];
    bool wireframe_;
    bool wireframeKeyPressed_;
    GLFWwindow *window_;
};

#endif