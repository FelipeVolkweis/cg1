#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "types/vec3.h"

/**
 * @struct ColorKey
 * @brief Represents a color as an RGB key for hashing.
 */
struct ColorKey {
    uint8_t r, g, b;

    bool operator==(const ColorKey &other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

namespace std {
template <> struct hash<ColorKey> {
    size_t operator()(const ColorKey &k) const {
        return (static_cast<size_t>(k.r) << 16) | (static_cast<size_t>(k.g) << 8) |
               static_cast<size_t>(k.b);
    }
};
} // namespace std

/**
 * @class Texture
 * @brief Provides utility methods for loading and creating textures in OpenGL.
 */
class Texture {
public:
    /**
     * @brief Loads a 2D texture from a file.
     * @param path The path to the texture file.
     * @return The OpenGL texture handle.
     */
    static uint32_t loadTexture(const std::string &path);

    /**
     * @brief Loads a cubemap texture from six image files.
     * @param faces A vector of six image file paths (px, nx, py, ny, pz, nz).
     * @return The OpenGL cubemap texture handle.
     */
    static uint32_t loadCubemap(const std::vector<std::string> &faces);
    static uint32_t createWhiteTexture();
    static uint32_t createGrayTexture();

    /**
     * @brief Creates a 1x1 texture of a specific color and transparency.
     * @param color The color of the texture.
     * @param transparency The alpha value (0-255).
     * @return The OpenGL texture handle.
     */
    static uint32_t createColorTexture(const Vec3 &color, uint8_t transparency = 255);

private:
    static std::unordered_map<ColorKey, uint32_t> colors_;
};

#endif