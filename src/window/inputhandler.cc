#include "inputhandler.h"

InputHandler::InputHandler(GLFWwindow *window)
    : window_(window), wireframe_(false), wireframeKeyPressed_(false), cycleClicked_(0),
      cycleKeyPressed_(false) {}

void InputHandler::handleWASD() {
    for (int i = 0; i < 4; i++) {
        wasd_[i] = 0;
    }

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        wasd_[W_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        wasd_[A_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        wasd_[S_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        wasd_[D_KEY] = 1;
}

void InputHandler::handleIJKL() {
    for (int i = 0; i < 4; i++) {
        ijkl_[i] = 0;
    }

    if (glfwGetKey(window_, GLFW_KEY_I) == GLFW_PRESS)
        ijkl_[I_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_J) == GLFW_PRESS)
        ijkl_[J_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_K) == GLFW_PRESS)
        ijkl_[K_KEY] = 1;
    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS)
        ijkl_[L_KEY] = 1;
}

void InputHandler::handleWireframe_P() {
    bool wireframeKey = glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS;
    if (wireframeKey && !wireframeKeyPressed_) {
        wireframe_ = !wireframe_;
    }
    wireframeKeyPressed_ = wireframeKey;
}

void InputHandler::handleCameraCycle_C() {
    bool cycleKey = glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS;
    if (cycleKey && !cycleKeyPressed_) {
        cycleClicked_++;
    }
    cycleKeyPressed_ = cycleKey;
}

void InputHandler::handleBrakes_SpaceBar() {
    bool spaceBar = glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS;
    spaceHeldDown_ = spaceBar;
}

Vec2 InputHandler::getMainYAxisInputVector() {
    return Vec2(wasd_[W_KEY], wasd_[S_KEY]);
}

Vec2 InputHandler::getMainXAxisInputVector() {
    return Vec2(wasd_[A_KEY], wasd_[D_KEY]);
}

Vec2 InputHandler::getSecondaryYAxisInputVector() {
    return Vec2(ijkl_[I_KEY], ijkl_[K_KEY]);
}

Vec2 InputHandler::getSecondaryXAxisInputVector() {
    return Vec2(ijkl_[J_KEY], ijkl_[L_KEY]);
}

bool InputHandler::getWireframeEnabled() {
    return wireframe_;
}

std::pair<int, int> InputHandler::getWindowDimensions() const {
    std::pair<int, int> dims;
    glfwGetWindowSize(window_, &dims.first, &dims.second);
    return dims;
}
