#include "inputhandler.h"

InputHandler::InputHandler(GLFWwindow *window)
    : window_(window), wireframe_(false), wireframeKeyPressed_(false) {}

void InputHandler::handleWASD() {
    for (int i = 0; i < 4; i++) {
        wasd_[i] = 0;
    }

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        wasd_[W_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        wasd_[A_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        wasd_[S_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        wasd_[D_KEY] = 1;
}

void InputHandler::handleWireframe_P() {
    bool wireframeKey = glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS;
    if (wireframeKey && !wireframeKeyPressed_) {
        wireframe_ = !wireframe_;
    }
    wireframeKeyPressed_ = wireframeKey;
}

Vec2 InputHandler::getYAxisInputVector() {
    return Vec2(wasd_[W_KEY], wasd_[S_KEY]);
}

Vec2 InputHandler::getXAxisInputVector() {
    return Vec2(wasd_[A_KEY], wasd_[D_KEY]);
}

bool InputHandler::getWireframeEnabled() {
    return wireframe_;
}