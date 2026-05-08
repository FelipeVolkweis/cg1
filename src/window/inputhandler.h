#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>

#include "types/vec2.h"

/**
 * @class InputHandler
 * @brief Handles user input from the keyboard and mouse using GLFW.
 *
 * This class tracks the state of specific keys and provides methods to query
 * input axes for movement and other actions.
 */
class InputHandler {
public:
    InputHandler(GLFWwindow *window);

    void handleWASD();
    void handleIJKL();
    void handleWireframe_P();
    void handleCameraCycle_C();
    void handleBrakes_SpaceBar();

    /**
     * @brief Gets the current dimensions of the window.
     * @return A pair containing width and height.
     */
    std::pair<int, int> getWindowDimensions() const;

    std::pair<int, int> getFrameBufferDimensions() const;

    /**
     * @brief Gets the input vector for the primary Y axis (W/S).
     * @return Vec2 representing the input.
     */
    Vec2 getMainYAxisInputVector();

    /**
     * @brief Gets the input vector for the primary X axis (A/D).
     * @return Vec2 representing the input.
     */
    Vec2 getMainXAxisInputVector();

    /**
     * @brief Gets the input vector for the secondary Y axis (I/K).
     * @return Vec2 representing the input.
     */
    Vec2 getSecondaryYAxisInputVector();

    /**
     * @brief Gets the input vector for the secondary X axis (J/L).
     * @return Vec2 representing the input.
     */
    Vec2 getSecondaryXAxisInputVector();

    /**
     * @brief Checks if wireframe mode is enabled.
     * @return true if wireframe is enabled, false otherwise.
     */
    bool getWireframeEnabled();

    /**
     * @brief Gets the number of times the camera cycle key was clicked.
     * @return Number of camera cycle requests.
     */
    uint32_t getNumberOfCameraCycleRequests() const {
        return cycleClicked_;
    }

    /**
     * @brief Checks if the space bar is currently held down.
     * @return true if held down, false otherwise.
     */
    bool isSpaceBarHeldDown() const {
        return spaceHeldDown_;
    }

    const int W_KEY = 0;
    const int A_KEY = 1;
    const int S_KEY = 2;
    const int D_KEY = 3;

    const int I_KEY = 0;
    const int J_KEY = 1;
    const int K_KEY = 2;
    const int L_KEY = 3;

private:
    int wasd_[4];
    int ijkl_[4];
    bool wireframe_;
    bool wireframeKeyPressed_;
    GLFWwindow *window_;

    uint32_t cycleClicked_ = 0;
    bool cycleKeyPressed_ = false;
    bool spaceHeldDown_ = false;
};

#endif