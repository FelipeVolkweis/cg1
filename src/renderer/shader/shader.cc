#include "shader.h"

#include <glad/glad.h>

#include "utils/fileutils.h"
#include "utils/logger.h"

namespace {
std::string getShaderTypeName(uint32_t type) {
    std::string typeName;

    if (type == GL_VERTEX_SHADER) {
        typeName = "vertex";
    } else if (type == GL_FRAGMENT_SHADER) {
        typeName = "fragment";
    } else if (type == GL_GEOMETRY_SHADER) {
        typeName = "geometry";
    }

    return typeName;
}
} // namespace

Shader::Shader(const char *vertexPath, const char *fragmentPath)
    : hasGeometry_(false), vertexPath_(vertexPath), fragmentPath_(fragmentPath) {}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
    : hasGeometry_(true), vertexPath_(vertexPath), fragmentPath_(fragmentPath),
      geometryPath_(geometryPath) {}

bool Shader::initialize() {
    std::string vertexShaderSource = readFile(vertexPath_);
    std::string fragmentShaderSource = readFile(fragmentPath_);

    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    id_ = glCreateProgram();
    glAttachShader(id_, vertexShader);
    glAttachShader(id_, fragmentShader);

    if (hasGeometry_) {
        std::string geometryShaderSource = readFile(geometryPath_);
        uint32_t geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryShaderSource.c_str());
        glAttachShader(id_, geometryShader);

        bool status = linkShaderProgram(id_);

        if (!status) {
            return false;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
    } else {
        bool status = linkShaderProgram(id_);

        if (!status) {
            return false;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    return true;
}

void Shader::use() {
    glUseProgram(id_);
}

void Shader::setInt(const std::string &name, int value) {
    findOrInsertLocation(name);
    glUniform1i(locations_[name], value);
}

void Shader::setFloat(const std::string &name, float value) {
    findOrInsertLocation(name);
    glUniform1f(locations_[name], value);
}

void Shader::setVec3(const std::string &name, const Vec3 &value) {
    findOrInsertLocation(name);
    glUniform3f(locations_[name], value.x(), value.y(), value.z());
}

void Shader::setMat4x4(const std::string &name, const Mat4x4 &value) {
    findOrInsertLocation(name);
    glUniformMatrix4fv(locations_[name], 1, GL_FALSE, value.data());
}

uint32_t Shader::compileShader(uint32_t type, const char *source) {
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    auto typeName = getShaderTypeName(type);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::Error("Error in ", typeName, " shader compilation - ", infoLog);
    }
    return shader;
}

bool Shader::linkShaderProgram(uint32_t shader) {
    glLinkProgram(shader);

    int success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        Logger::Error("Error in shader program linkage\n", infoLog);
        return false;
    }

    return true;
}

bool Shader::findOrInsertLocation(const std::string &name) {
    bool found = locations_.find(name) != locations_.end();

    if (!found) {
        int location = glGetUniformLocation(id_, name.c_str());
        locations_[name] = location;
    }

    return found;
}