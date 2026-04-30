cmake_minimum_required(VERSION 3.16)
project(ComputerGraphics LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Bullet REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cc")
list(APPEND SOURCES "3rdparty/glad/src/glad.c")
add_executable(cg ${SOURCES})

target_link_libraries(cg PRIVATE 
    OpenGL::GL
    glfw
    Eigen3::Eigen
    ${BULLET_LIBRARIES}
)

target_include_directories(cg PRIVATE 
    src
    3rdparty/glad/include
)

