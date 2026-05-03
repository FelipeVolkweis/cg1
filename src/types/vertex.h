#ifndef VERTEX_H
#define VERTEX_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
    Vec4 color;

    Vertex(const Vec3 &pos, const Vec3 &norm = Vec3::Zero(), const Vec2 &tex = Vec2::Zero(),
           const Vec4 &col = Vec4(1, 1, 1, 1))
        : position(pos), normal(norm), texCoords(tex), color(col) {}
};

#endif