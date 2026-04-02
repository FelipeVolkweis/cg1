#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

namespace Logger {
template <typename... Args> void Info(Args &&...args) {
    std::cout << "[INFO]  ";
    (std::cout << ... << std::forward<Args>(args)) << '\n';
}

template <typename... Args> void Warn(Args &&...args) {
    std::cerr << "[WARN]  ";
    (std::cerr << ... << std::forward<Args>(args)) << '\n';
}

template <typename... Args> void Error(Args &&...args) {
    std::cerr << "[ERROR] ";
    (std::cerr << ... << std::forward<Args>(args)) << '\n';
}
} // namespace Logger

#endif