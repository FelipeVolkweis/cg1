#ifndef VERTEX_H
#define VERTEX_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

/**
 * @struct Vertex
 * @brief Represents a single vertex in 3D space with attributes.
 */
struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;

    /**
     * @brief Constructs a new Vertex object.
     * @param pos Initial position.
     * @param norm Initial normal (defaults to zero).
     * @param tex Initial texture coordinates (defaults to zero).
     */
    Vertex(const Vec3 &pos, const Vec3 &norm = Vec3::Zero(), const Vec2 &tex = Vec2::Zero())
        : position(pos), normal(norm), texCoords(tex) {}
};

#endif