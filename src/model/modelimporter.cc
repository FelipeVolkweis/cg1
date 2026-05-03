#include "modelimporter.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <filesystem>

#include "renderer/texture.h"
#include "shapes/baseshape.h"
#include "tiny_obj_loader.h"
#include "utils/logger.h"

// https://github.com/tinyobjloader/tinyobjloader
void ModelImporter::loadObjInto(const std::string &filePath, BaseShape *shape) {
    tinyobj::ObjReaderConfig reader_config;
    std::filesystem::path p(filePath);
    std::string directory = p.parent_path().string();
    if (!directory.empty()) {
        directory += "/";
    }
    reader_config.mtl_search_path = directory;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filePath, reader_config)) {
        if (!reader.Error().empty()) {
            Logger::Error("TinyObjReader: ", reader.Error());
        }
        return;
    }

    if (!reader.Warning().empty()) {
        Logger::Warn("TinyObjReader: ", reader.Warning());
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    uint32_t textureId = 0;

    for (const auto &mat : materials) {
        if (!mat.diffuse_texname.empty()) {
            std::string texPath = directory + mat.diffuse_texname;
            textureId = Texture::loadTexture(texPath);
            if (textureId != 0) {
                break;
            }
        }
    }

    shape->setTextureId(textureId);

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                Vec3 pos(vx, vy, vz);
                Vec3 norm(0, 0, 0);
                Vec2 tex(0, 0);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    norm = Vec3(nx, ny, nz);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    tex = Vec2(tx, ty);
                }

                Vec3 col(1, 1, 1);
                int material_id = shapes[s].mesh.material_ids[f];
                if (material_id >= 0 && material_id < materials.size()) {
                    col = Vec3(materials[material_id].diffuse[0], materials[material_id].diffuse[1],
                               materials[material_id].diffuse[2]);
                }

                shape->addVertex(Vertex(pos, norm, tex, col));

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;
        }
    }
}