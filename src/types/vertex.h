#ifndef VERTEX_H
#define VERTEX_H

#include "vec2.h"
#include "vec3.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
    Vec3 color;

    Vertex(const Vec3 &pos, const Vec3 &norm = Vec3::Zero(), const Vec2 &tex = Vec2::Zero(),
           const Vec3 &col = Vec3(1, 1, 1))
        : position(pos), normal(norm), texCoords(tex), color(col) {}
};

#endif