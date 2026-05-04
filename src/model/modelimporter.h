#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

#include <string>

#include "renderer/renderable.h"

struct ModelPackage {
    ModelPackage() {
        vertices = std::make_shared<std::vector<Vertex>>();
        groups = std::make_shared<std::vector<MeshGroup>>();
    }

    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<MeshGroup>> groups;
    bool valid = false;
};

class ModelImporter {
public:
    static ModelPackage loadObjInto(const std::string &filePath);

private:
};

#endif