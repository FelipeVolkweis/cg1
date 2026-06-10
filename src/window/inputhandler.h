#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "types/vec2.h"

struct GLFWwindow;

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
    void handleLightControls();

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

    /**
     * @brief Gets the currently selected light index (0-9), or -1 if none.
     * @return The selected light index.
     */
    int getSelectedLightIndex() const {
        return selectedLightIndex_;
    }

    /**
     * @brief Returns true once per press of the toggle key (U).
     * Resets after being read.
     * @return true if toggle was requested.
     */
    bool consumeLightToggleRequest();

    /**
     * @brief Gets the ambient adjustment direction: +1, -1, or 0.
     */
    int getLightAmbientDelta() const {
        return lightAmbientDelta_;
    }

    /**
     * @brief Gets the diffuse adjustment direction: +1, -1, or 0.
     */
    int getLightDiffuseDelta() const {
        return lightDiffuseDelta_;
    }

    /**
     * @brief Gets the specular adjustment direction: +1, -1, or 0.
     */
    int getLightSpecularDelta() const {
        return lightSpecularDelta_;
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

    int selectedLightIndex_ = -1;
    bool lightToggleRequested_ = false;
    bool lightToggleKeyPressed_ = false;
    int lightAmbientDelta_ = 0;
    int lightDiffuseDelta_ = 0;
    int lightSpecularDelta_ = 0;
};

#endif