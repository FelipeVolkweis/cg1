#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "types/vec3.h"

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

class Texture {
public:
    static uint32_t loadTexture(const std::string &path);
    static uint32_t loadCubemap(const std::vector<std::string> &faces);
    static uint32_t createWhiteTexture();
    static uint32_t createColorTexture(const Vec3 &color, uint8_t transparency = 255);

private:
    static std::unordered_map<ColorKey, uint32_t> colors_;
};

#endif