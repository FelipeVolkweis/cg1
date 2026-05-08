#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "types/mat4x4.h"
#include "types/vec3.h"

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath);

    bool initialize();
    void use();

    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, const Vec3 &value);
    void setMat4x4(const std::string &name, const Mat4x4 &value);

    uint32_t getId() const { return id_; }

private:
    uint32_t id_;
    bool hasGeometry_;

    const char *vertexPath_;
    const char *fragmentPath_;
    const char *geometryPath_;

    std::unordered_map<std::string, int> locations_;

    uint32_t compileShader(uint32_t type, const char *source);
    bool linkShaderProgram(uint32_t shader);
    bool findOrInsertLocation(const std::string &name);
};

#endif