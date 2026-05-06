#include "cylinder.h"

#include <cmath>

#include <yaml-cpp/yaml.h>

Cylinder::Cylinder(float radius, float height, int sectors) {
    generateVertices(radius, height, sectors);
}

void Cylinder::parse(const YAML::Node &node) {
    float r = 1.0f, h = 2.0f;
    int s = 36;
    if (node["radius"])
        r = node["radius"].as<float>();
    if (node["height"])
        h = node["height"].as<float>();
    if (node["sectors"])
        s = node["sectors"].as<int>();
    generateVertices(r, h, s);
    parseCommon(node);
}

void Cylinder::generateVertices(float radius, float height, int sectors) {
    initializeCommon();
    vertices_->clear();
    float sectorStep = 2 * M_PI / sectors;
    float h2 = height / 2.0f;

    for (int i = 0; i < sectors; ++i) {
        float a1 = i * sectorStep;
        float a2 = (i + 1) * sectorStep;

        Vec3 v1(radius * cos(a1), radius * sin(a1), h2);
        Vec3 v2(radius * cos(a2), radius * sin(a2), h2);
        Vec3 v3(radius * cos(a1), radius * sin(a1), -h2);
        Vec3 v4(radius * cos(a2), radius * sin(a2), -h2);

        Vec3 n1(v1.x(), v1.y(), 0.0f); n1.normalize();
        Vec3 n2(v2.x(), v2.y(), 0.0f); n2.normalize();
        Vec3 n3(v3.x(), v3.y(), 0.0f); n3.normalize();
        Vec3 n4(v4.x(), v4.y(), 0.0f); n4.normalize();

        addVertex(Vertex(v1, n1));
        addVertex(Vertex(v3, n3));
        addVertex(Vertex(v2, n2));
        addVertex(Vertex(v2, n2));
        addVertex(Vertex(v3, n3));
        addVertex(Vertex(v4, n4));

        Vec3 topN(0, 0, 1);
        addVertex(Vertex(Vec3(0, 0, h2), topN));
        addVertex(Vertex(v1, topN));
        addVertex(Vertex(v2, topN));
        
        Vec3 botN(0, 0, -1);
        addVertex(Vertex(Vec3(0, 0, -h2), botN));
        addVertex(Vertex(v4, botN));
        addVertex(Vertex(v3, botN));
    }
}
