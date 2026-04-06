#ifndef WINDOW_H
#define WINDOW_H

#include "renderer/renderer.h"

struct GLFWwindow;
class Scene;

class Window {
public:
    Window() = default;

    bool initialize();
    bool finish();
    bool loop(Scene *scene = nullptr);

    Renderer &getRenderer() {
        return renderer_;
    }

private:
    void processInput();

    GLFWwindow *window_;
    Renderer renderer_;
    bool wireframe_ = false;
    bool wireframeKeyPressed_ = false;
    float transX_ = 0.0f;
    float transY_ = 0.0f;
    float transZ_ = 0.0f;
    float scale_ = 1.0f;
    float rotY_ = 0.0f;
};

#endif