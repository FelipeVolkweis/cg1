#ifndef WINDOW_H
#define WINDOW_H

#include <memory>

#include "renderer/renderer.h"

struct GLFWwindow;
struct InputHandler;
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
    bool loop(std::unique_ptr<Scene> scene);

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
    InputHandler *inputHandler_;
    Renderer renderer_;
};

#endif