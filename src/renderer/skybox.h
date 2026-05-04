#ifndef SKYBOX_H
#define SKYBOX_H

#include <cstdint>
#include <string>
#include <vector>

#include "types/mat4x4.h"

/**
 * @class Skybox
 * @brief Represents a skybox environment map for the scene.
 */
// https://youtu.be/8sVvxeKI9Pk
class Skybox {
public:
    /**
     * @brief Constructs a Skybox with the given face textures.
     * @param faces A vector of six image file paths for the cubemap faces.
     */
    Skybox(const std::vector<std::string> &faces);
    ~Skybox();

    /**
     * @brief Renders the skybox.
     * @param view The view matrix of the camera.
     * @param projection The projection matrix.
     */
    void render(const Mat4x4 &view, const Mat4x4 &projection);

private:
    uint32_t textureId_;
    uint32_t vao_, vbo_, ebo_;
    uint32_t shaderProgram_;

    uint32_t compileShader(uint32_t type, const char *source);

    /**
     * @brief Sets up the geometry (cube) for the skybox.
     */
    void setupGeometry();
    void setupShaders();
};

#endif
