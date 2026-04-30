#include "cylinder.h"

#include <cmath>

Cylinder::Cylinder(float radius, float height, int sectors) {
    generateVertices(radius, height, sectors);
}

Transformation Cylinder::parse(std::istream &is) {
    float r, h;
    int s;
    Transformation transform;
    if (is >> r >> h >> s) {
        generateVertices(r, h, s);
        transform = parseCommon(is);
    }

    return transform;
}

void Cylinder::generateVertices(float radius, float height, int sectors) {
    vertices_.clear();
    float sectorStep = 2 * M_PI / sectors;
    float h2 = height / 2.0f;

    for (int i = 0; i < sectors; ++i) {
        float a1 = i * sectorStep;
        float a2 = (i + 1) * sectorStep;

        Vec3 v1(radius * cos(a1), radius * sin(a1), h2);
        Vec3 v2(radius * cos(a2), radius * sin(a2), h2);
        Vec3 v3(radius * cos(a1), radius * sin(a1), -h2);
        Vec3 v4(radius * cos(a2), radius * sin(a2), -h2);

        addVertex(v1);
        addVertex(v3);
        addVertex(v2);
        addVertex(v2);
        addVertex(v3);
        addVertex(v4);

        addVertex(Vec3(0, 0, h2));
        addVertex(v1);
        addVertex(v2);
        addVertex(Vec3(0, 0, -h2));
        addVertex(v4);
        addVertex(v3);
    }
}