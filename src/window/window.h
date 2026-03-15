#ifndef WINDOW_H
#define WINDOW_H

#include "renderer/renderer.h"

struct GLFWwindow;

class Window {
public:
    Window() = default;

    bool initialize();
    bool finish();
    bool loop();
private:
    GLFWwindow *window_;
    Renderer renderer_;
};


#endif