#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

#include <string>

#include "renderer/meshes/renderablemesh.h"

/**
 * @brief Structure containing the loaded model data.
 */
struct ModelPackage {
    ModelPackage() {
        vertices = std::make_shared<std::vector<Vertex>>();
        groups = std::make_shared<std::vector<MeshGroup>>();
    }

    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<MeshGroup>> groups;
    bool valid = false;
};

/**
 * @brief Class for importing 3D models from files.
 */
class ModelImporter {
public:
    /**
     * @brief Loads an OBJ model from the specified file path.
     *
     * @param filePath The path to the OBJ file.
     * @return ModelPackage The loaded model data.
     */
    static ModelPackage loadObjInto(const std::string &filePath);

private:
};

#endif
