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

    window_ = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, resizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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

