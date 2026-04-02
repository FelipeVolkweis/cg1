#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

bool Window::loop() {
    while (!glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderer_.render();

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    return true;
}
