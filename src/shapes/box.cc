#include "box.h"

#include <yaml-cpp/yaml.h>

Box::Box(float width, float height, float depth) {
    generateVertices(width, height, depth);
}

void Box::parse(const YAML::Node &node) {
    float w = 1.0f, h = 1.0f, d = 1.0f;
    if (node["width"])
        w = node["width"].as<float>();
    if (node["height"])
        h = node["height"].as<float>();
    if (node["depth"])
        d = node["depth"].as<float>();
    generateVertices(w, h, d);
    parseCommon(node);
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