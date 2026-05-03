#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>

class Texture {
public:
    static uint32_t loadTexture(const std::string &path);
    static uint32_t createWhiteTexture();
};

#endif