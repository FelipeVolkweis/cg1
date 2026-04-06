#include "cone.h"
#include <cmath>

Cone::Cone(float radius, float height, int sectors) {
    generateVertices(radius, height, sectors);
}

void Cone::parse(std::istream &is) {
    float radius, height;
    int sectors;

    if (is >> radius >> height >> sectors) {
        generateVertices(radius, height, sectors);
        parseCommon(is);
    }
}

void Cone::generateVertices(float radius, float height, int sectors) {
    vertices_.clear();

    float sectorStep = 2 * M_PI / sectors;
    float halfH = height / 2.0f;

    Vec3 tip(0, 0, halfH);
    Vec3 baseCenter(0, 0, -halfH);

    for (int i = 0; i < sectors; ++i) {
        float angle1 = i * sectorStep;
        float angle2 = (i + 1) * sectorStep;

        Vec3 b1(radius * std::cos(angle1), radius * std::sin(angle1), -halfH);
        Vec3 b2(radius * std::cos(angle2), radius * std::sin(angle2), -halfH);

        addVertex(tip);
        addVertex(b1);
        addVertex(b2);

        addVertex(baseCenter);
        addVertex(b2);
        addVertex(b1);
    }
}