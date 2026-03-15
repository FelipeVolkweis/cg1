cmake_minimum_required(VERSION 3.16)
project(ComputerGraphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(cg src/main.cc)

target_link_libraries(cg PRIVATE 
    OpenGL::GL
    glfw
    Eigen3::Eigen
)

target_include_directories(cg PRIVATE src)
