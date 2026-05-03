#include "modelimporter.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <filesystem>
#include <map>
#include <vector>

#include "renderer/texture.h"
#include "shapes/baseshape.h"
#include "tiny_obj_loader.h"
#include "utils/logger.h"

// https://github.com/tinyobjloader/tinyobjloader
namespace {
bool parseObjFile(const std::string &filePath, std::string &outDirectory,
                  tinyobj::ObjReader &reader) {
    tinyobj::ObjReaderConfig reader_config;
    std::filesystem::path p(filePath);
    outDirectory = p.parent_path().string();
    if (!outDirectory.empty()) {
        outDirectory += "/";
    }
    reader_config.mtl_search_path = outDirectory;

    if (!reader.ParseFromFile(filePath, reader_config)) {
        if (!reader.Error().empty()) {
            Logger::Error("TinyObjReader: ", reader.Error());
        }
        return false;
    }

    if (!reader.Warning().empty()) {
        Logger::Warn("TinyObjReader: ", reader.Warning());
    }

    return true;
}

std::map<int, uint32_t> loadMaterialTextures(const std::vector<tinyobj::material_t> &materials,
                                             const std::string &directory) {
    std::map<int, uint32_t> materialToTexture;
    for (size_t i = 0; i < materials.size(); ++i) {
        if (!materials[i].diffuse_texname.empty()) {
            std::string texPath = directory + materials[i].diffuse_texname;
            uint32_t tid = Texture::loadTexture(texPath);
            if (tid != 0) {
                materialToTexture[i] = tid;
            }
        }
    }
    return materialToTexture;
}

std::vector<std::vector<size_t>> calculateFaceOffsets(const std::vector<tinyobj::shape_t> &shapes) {
    std::vector<std::vector<size_t>> shapeFaceOffsets(shapes.size());
    for (size_t s = 0; s < shapes.size(); s++) {
        shapeFaceOffsets[s].push_back(0);
        size_t offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            offset += size_t(shapes[s].mesh.num_face_vertices[f]);
            shapeFaceOffsets[s].push_back(offset);
        }
    }
    return shapeFaceOffsets;
}

std::map<int, std::vector<std::pair<size_t, size_t>>>
groupFacesByMaterial(const std::vector<tinyobj::shape_t> &shapes) {
    std::map<int, std::vector<std::pair<size_t, size_t>>> materialGroups;
    for (size_t s = 0; s < shapes.size(); s++) {
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int matId = shapes[s].mesh.material_ids[f];
            materialGroups[matId].push_back({s, f});
        }
    }
    return materialGroups;
}

void buildShapeGeometry(
    BaseShape *shape, const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
    const std::vector<tinyobj::material_t> &materials,
    const std::map<int, uint32_t> &materialToTexture,
    const std::vector<std::vector<size_t>> &shapeFaceOffsets,
    const std::map<int, std::vector<std::pair<size_t, size_t>>> &materialGroups) {
    shape->clearGroups();
    size_t vertexOffset = 0;

    for (auto const &[matId, faces] : materialGroups) {
        size_t groupStart = vertexOffset;
        size_t groupCount = 0;
        bool isTransparent = false;

        if (matId >= 0 && matId < (int)materials.size()) {
            if (materials[matId].dissolve < 1.0f) {
                isTransparent = true;
            }
        }

        for (auto const &faceRef : faces) {
            size_t s = faceRef.first;
            size_t f = faceRef.second;
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            size_t index_offset = shapeFaceOffsets[s][f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                Vec3 pos(vx, vy, vz);

                Vec3 norm(0, 0, 0);
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    norm = Vec3(nx, ny, nz);
                }

                Vec2 tex(0, 0);
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    tex = Vec2(tx, ty);
                }

                Vec4 col(1, 1, 1, 1);
                if (matId >= 0 && matId < (int)materials.size()) {
                    col = Vec4(materials[matId].diffuse[0], materials[matId].diffuse[1],
                               materials[matId].diffuse[2], materials[matId].dissolve);
                }

                shape->addVertex(Vertex(pos, norm, tex, col));
                groupCount++;
                vertexOffset++;
            }
        }

        uint32_t tid = 0;
        if (materialToTexture.count(matId)) {
            tid = materialToTexture.at(matId);
        }

        shape->addGroup({tid, groupStart, groupCount, isTransparent});
    }
}

} // namespace

void ModelImporter::loadObjInto(const std::string &filePath, BaseShape *shape) {
    std::string directory;
    tinyobj::ObjReader reader;

    if (!parseObjFile(filePath, directory, reader)) {
        return;
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    auto materialToTexture = loadMaterialTextures(materials, directory);
    auto shapeFaceOffsets = calculateFaceOffsets(shapes);
    auto materialGroups = groupFacesByMaterial(shapes);

    buildShapeGeometry(shape, attrib, shapes, materials, materialToTexture, shapeFaceOffsets,
                       materialGroups);
}