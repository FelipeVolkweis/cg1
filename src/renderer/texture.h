#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>
#include <vector>

class Texture {
public:
    static uint32_t loadTexture(const std::string &path);
    static uint32_t loadCubemap(const std::vector<std::string> &faces);
    static uint32_t createWhiteTexture();
};

#endif