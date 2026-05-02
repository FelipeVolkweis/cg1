#include "window.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera/camera.h"
#include "inputhandler.h"
#include "scene/scene.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"

#define WINDOW_WIDTH 960.0f
#define WINDOW_HEIGHT 540.0f

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

    window_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", NULL, NULL);
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

    Perspective p = {45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f};
    LookAt l = {Vec3(0, 0, 3), Vec3(0, 0, 0), Vec3(0, 1, 0)};
    mainCamera_ = std::make_unique<Camera>(p, l);

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

        renderer_.setActiveCamera(mainCamera_);

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