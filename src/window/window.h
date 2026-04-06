#ifndef WINDOW_H
#define WINDOW_H

#include "renderer/renderer.h"

struct GLFWwindow;
class Scene;

/**
 * @class Window
 * @brief Manages the GLFW window, input handling, and the main application loop.
 */
class Window {
public:
    Window() = default;

    /**
     * @brief Initializes GLFW and creates the window.
     * @return true if initialization was successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Cleans up GLFW and window resources.
     * @return true if cleanup was successful, false otherwise.
     */
    bool finish();

    /**
     * @brief Executes the main application loop.
     * @param scene Pointer to the scene to be rendered.
     * @return true if the loop completed normally, false on error.
     */
    bool loop(Scene *scene = nullptr);

    /**
     * @brief Returns a reference to the renderer.
     * @return Reference to the Renderer instance.
     */
    Renderer &getRenderer() {
        return renderer_;
    }

private:
    /**
     * @brief Processes keyboard and mouse input.
     */
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