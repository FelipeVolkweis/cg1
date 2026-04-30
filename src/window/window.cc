#include "window.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

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
    return true;
}

void Window::processInput() {
    float moveSpeed = 0.05f;
    float scaleSpeed = 0.05f;
    float rotSpeed = 2.0f;

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        transY_ += moveSpeed;
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        transY_ -= moveSpeed;
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        transX_ -= moveSpeed;
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        transX_ += moveSpeed;

    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS)
        scale_ += scaleSpeed;
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS)
        scale_ -= scaleSpeed;

    if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS)
        rotY_ += rotSpeed;
    if (glfwGetKey(window_, GLFW_KEY_T) == GLFW_PRESS)
        rotY_ -= rotSpeed;

    bool wireframeKey = glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS;
    if (wireframeKey && !wireframeKeyPressed_) {
        wireframe_ = !wireframe_;
        if (wireframe_) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    wireframeKeyPressed_ = wireframeKey;
}

bool Window::loop(Scene *scene) {
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