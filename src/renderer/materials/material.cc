#include "material.h"

#include <yaml-cpp/yaml.h>

#include "renderer/textures/texture.h"
#include "utils/logger.h"

Material::Material(uint32_t diffuseTextureId, uint32_t specularTextureId, float shininess,
                   float dissolve)
    : diffuseTextureId_(diffuseTextureId), specularTextureId_(specularTextureId),
      shininess_(shininess), dissolve_(dissolve) {}

Material::Material(const Vec3 &diffuse, const Vec3 &specular, float shininess, float dissolve)
    : diffuseTextureId_(Texture::createColorTexture(diffuse)),
      specularTextureId_(Texture::createColorTexture(specular)), shininess_(shininess),
      dissolve_(dissolve) {}

Material Material::fromYaml(const std::string &name) {
    std::string path = "data/materials/" + name + ".yaml";

    YAML::Node data;
    try {
        data = YAML::LoadFile(path);
    } catch (const std::exception &e) {
        Logger::Error("Failed to load material: ", path, " — ", e.what());
        return Material(Vec3(0.8f, 0.8f, 0.8f), Vec3(1.0f, 1.0f, 1.0f), 32.0f);
    }

    Vec3 diffuse(0.8f, 0.8f, 0.8f);
    if (data["diffuse"] && data["diffuse"].IsSequence())
        diffuse = Vec3(data["diffuse"][0].as<float>(), data["diffuse"][1].as<float>(),
                       data["diffuse"][2].as<float>());

    Vec3 specular(1.0f, 1.0f, 1.0f);
    if (data["specular"] && data["specular"].IsSequence())
        specular = Vec3(data["specular"][0].as<float>(), data["specular"][1].as<float>(),
                        data["specular"][2].as<float>());

    float shininess = 32.0f;
    if (data["shininess"])
        shininess = data["shininess"].as<float>();

    float dissolve = 1.0f;
    if (data["dissolve"])
        dissolve = data["dissolve"].as<float>();

    return Material(diffuse, specular, shininess, dissolve);
}
