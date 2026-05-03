#ifndef SKYBOX_H
#define SKYBOX_H

#include <cstdint>
#include <string>
#include <vector>

#include "types/mat4x4.h"

// https://youtu.be/8sVvxeKI9Pk
class Skybox {
public:
    Skybox(const std::vector<std::string> &faces);
    ~Skybox();

    void render(const Mat4x4 &view, const Mat4x4 &projection);

private:
    uint32_t textureId_;
    uint32_t vao_, vbo_, ebo_;
    uint32_t shaderProgram_;

    uint32_t compileShader(uint32_t type, const char *source);
    void setupGeometry();
    void setupShaders();
};

#endif
