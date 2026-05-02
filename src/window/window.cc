#include "window.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "inputhandler.h"
#include "scene/scene.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"

void resizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool Window::initialize() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(640, 640, "OpenGL Window", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        return false;
    }
    inputHandler_ = new InputHandler(window_);
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, resizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    if (!renderer_.initialize()) {
        return false;
    }

    return true;
}

bool Window::finish() {
    glfwTerminate();
    delete inputHandler_;
    return true;
}

void Window::processInput() {
    inputHandler_->handleWASD();
    inputHandler_->handleWireframe_P();

    bool wireframeKey = inputHandler_->getWireframeEnabled();
    if (wireframeKey) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

bool Window::loop(std::unique_ptr<Scene> scene) {
    while (!glfwWindowShouldClose(window_)) {
        processInput();

        if (scene) {
            renderer_.clear();
            scene->populateRenderer(renderer_);
        }

        renderer_.render();

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return true;
}