#include "semisphere.h"

#include <cmath>

#include <yaml-cpp/yaml.h>

SemiSphere::SemiSphere(float radius, int sectors, int stacks) {
    generateVertices(radius, sectors, stacks);
}

void SemiSphere::parse(const YAML::Node &node) {
    float radius = 1.0f;
    int sectors = 36, stacks = 18;

    if (node["radius"])
        radius = node["radius"].as<float>();
    if (node["sectors"])
        sectors = node["sectors"].as<int>();
    if (node["stacks"])
        stacks = node["stacks"].as<int>();

    generateVertices(radius, sectors, stacks);
    parseCommon(node);
}

void SemiSphere::generateVertices(float radius, int sectors, int stacks) {
    initializeCommon();
    vertices_->clear();

    float sectorStep = 2 * M_PI / sectors;
    float stackStep = (M_PI / 2) / stacks;

    for (int i = 0; i < stacks; ++i) {
        float stackAngle1 = M_PI / 2 - i * stackStep;
        float stackAngle2 = M_PI / 2 - (i + 1) * stackStep;

        float xy1 = radius * cos(stackAngle1);
        float z1 = radius * sin(stackAngle1);

        float xy2 = radius * cos(stackAngle2);
        float z2 = radius * sin(stackAngle2);

        for (int j = 0; j < sectors; ++j) {
            float sectorAngle1 = j * sectorStep;
            float sectorAngle2 = (j + 1) * sectorStep;

            Vec3 v1(xy1 * cos(sectorAngle1), xy1 * sin(sectorAngle1), z1);
            Vec3 v2(xy1 * cos(sectorAngle2), xy1 * sin(sectorAngle2), z1);
            Vec3 v3(xy2 * cos(sectorAngle1), xy2 * sin(sectorAngle1), z2);
            Vec3 v4(xy2 * cos(sectorAngle2), xy2 * sin(sectorAngle2), z2);

            addVertex(Vertex(v1, v1.normalized()));
            addVertex(Vertex(v3, v3.normalized()));
            addVertex(Vertex(v2, v2.normalized()));
            addVertex(Vertex(v2, v2.normalized()));
            addVertex(Vertex(v3, v3.normalized()));
            addVertex(Vertex(v4, v4.normalized()));

            if (i == (stacks - 1)) {
                Vec3 center(0, 0, z2);
                Vec3 baseN(0, 0, -1);
                addVertex(Vertex(center, baseN));
                addVertex(Vertex(v4, baseN));
                addVertex(Vertex(v3, baseN));
            }
        }
    }
}
