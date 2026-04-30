#include "box.h"

Box::Box(float width, float height, float depth) {
    generateVertices(width, height, depth);
}

Transformation Box::parse(std::istream &is) {
    float w, h, d;
    Transformation transform;
    if (is >> w >> h >> d) {
        generateVertices(w, h, d);
        transform = parseCommon(is);
    }
    return transform;
}

void Box::generateVertices(float width, float height, float depth) {
    vertices_.clear();

    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    float d2 = depth / 2.0f;

    auto addFace = [&](Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4) {
        addVertex(v1);
        addVertex(v2);
        addVertex(v3);

        addVertex(v1);
        addVertex(v3);
        addVertex(v4);
    };

    addFace({-w2, -h2, d2}, {w2, -h2, d2}, {w2, h2, d2}, {-w2, h2, d2});
    addFace({w2, -h2, -d2}, {-w2, -h2, -d2}, {-w2, h2, -d2}, {w2, h2, -d2});
    addFace({-w2, h2, d2}, {w2, h2, d2}, {w2, h2, -d2}, {-w2, h2, -d2});
    addFace({-w2, -h2, -d2}, {w2, -h2, -d2}, {w2, -h2, d2}, {-w2, -h2, d2});
    addFace({w2, -h2, d2}, {w2, -h2, -d2}, {w2, h2, -d2}, {w2, h2, d2});
    addFace({-w2, -h2, -d2}, {-w2, -h2, d2}, {-w2, h2, d2}, {-w2, h2, -d2});
}