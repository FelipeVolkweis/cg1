#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <fstream>
#include <sstream>
#include <string>

#include "utils/logger.h"

/**
 * @brief Reads the contents of a file into a string.
 * @param filePath The path to the file.
 * @return The contents of the file, or an empty string if reading fails.
 */
inline std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::Error("Failed to open file: " + filePath);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

#endif
