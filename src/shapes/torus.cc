#include "torus.h"

#include <cmath>

#include <yaml-cpp/yaml.h>

Torus::Torus(float innerRadius, float outerRadius, int sides, int rings) {
    generateVertices(innerRadius, outerRadius, sides, rings);
}

void Torus::parse(const YAML::Node &node) {
    float inner = 0.5f, outer = 1.0f;
    int sides = 36, rings = 36;

    if (node["innerRadius"])
        inner = node["innerRadius"].as<float>();
    if (node["outerRadius"])
        outer = node["outerRadius"].as<float>();
    if (node["sides"])
        sides = node["sides"].as<int>();
    if (node["rings"])
        rings = node["rings"].as<int>();

    generateVertices(inner, outer, sides, rings);
    parseCommon(node);
}

void Torus::generateVertices(float innerRadius, float outerRadius, int sides, int rings) {
    initializeCommon();
    vertices_->clear();

    float ringStep = 2 * M_PI / rings;
    float sideStep = 2 * M_PI / sides;

    float tubeRadius = (outerRadius - innerRadius) / 2.0f;
    float mainRadius = innerRadius + tubeRadius;

    for (int i = 0; i < rings; ++i) {
        float u1 = i * ringStep;
        float u2 = (i + 1) * ringStep;

        for (int j = 0; j < sides; ++j) {
            float v1 = j * sideStep;
            float v2 = (j + 1) * sideStep;

            auto getPos = [&](float u, float v) {
                float x = (mainRadius + tubeRadius * std::cos(v)) * std::cos(u);
                float y = (mainRadius + tubeRadius * std::cos(v)) * std::sin(u);
                float z = tubeRadius * std::sin(v);
                return Vec3(x, y, z);
            };

            Vec3 p1 = getPos(u1, v1);
            Vec3 p2 = getPos(u1, v2);
            Vec3 p3 = getPos(u2, v1);
            Vec3 p4 = getPos(u2, v2);

            addVertex(p1);
            addVertex(p2);
            addVertex(p3);
            addVertex(p2);
            addVertex(p4);
            addVertex(p3);
        }
    }
}