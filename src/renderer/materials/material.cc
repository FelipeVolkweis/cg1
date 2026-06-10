#include "material.h"

#include <yaml-cpp/yaml.h>

#include "renderer/textures/texture.h"
#include "utils/logger.h"

Material::Material(uint32_t diffuseMap, uint32_t specularMap, const Vec3 &ambientColor,
                   const Vec3 &diffuseColor, const Vec3 &specularColor, float shininess,
                   float dissolve)
    : diffuseMap_(diffuseMap), specularMap_(specularMap), ambientColor_(ambientColor),
      diffuseColor_(diffuseColor), specularColor_(specularColor), shininess_(shininess),
      dissolve_(dissolve) {}

Material Material::fromYaml(const std::string &name) {
    std::string path = "data/materials/" + name + ".yaml";

    YAML::Node data;
    try {
        data = YAML::LoadFile(path);
    } catch (const std::exception &e) {
        Logger::Error("Failed to load material: ", path, " — ", e.what());
        return Material(Texture::createWhiteTexture(), Texture::createWhiteTexture(),
                        Vec3(0.5f, 0.5f, 0.5f), Vec3(0.8f, 0.8f, 0.8f), Vec3(1.0f, 1.0f, 1.0f),
                        32.0f);
    }

    Vec3 ambient(0.5f, 0.5f, 0.5f);
    if (data["ambient"] && data["ambient"].IsSequence())
        ambient = Vec3(data["ambient"][0].as<float>(), data["ambient"][1].as<float>(),
                       data["ambient"][2].as<float>());

    Vec3 diffuse(0.8f, 0.8f, 0.8f);
    if (data["diffuse"] && data["diffuse"].IsSequence())
        diffuse = Vec3(data["diffuse"][0].as<float>(), data["diffuse"][1].as<float>(),
                       data["diffuse"][2].as<float>());

    if (!data["ambient"] && data["diffuse"]) {
        ambient = diffuse;
    }

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

    return Material(Texture::createWhiteTexture(), Texture::createWhiteTexture(), ambient, diffuse,
                    specular, shininess, dissolve);
}

Material Material::overlayYaml(const Material &base, const std::string &name) {
    std::string path = "data/materials/" + name + ".yaml";

    YAML::Node data;
    try {
        data = YAML::LoadFile(path);
    } catch (const std::exception &e) {
        Logger::Error("Failed to load material: ", path, " — ", e.what());
        return base;
    }

    Vec3 ambient = base.getAmbientColor();
    if (data["ambient"] && data["ambient"].IsSequence()) {
        ambient = Vec3(data["ambient"][0].as<float>(), data["ambient"][1].as<float>(),
                       data["ambient"][2].as<float>());
    }

    Vec3 diffuse = base.getDiffuseColor();
    if (data["diffuse"] && data["diffuse"].IsSequence()) {
        diffuse = Vec3(data["diffuse"][0].as<float>(), data["diffuse"][1].as<float>(),
                       data["diffuse"][2].as<float>());
        if (!data["ambient"]) {
            ambient = diffuse;
        }
    }

    Vec3 specular = base.getSpecularColor();
    if (data["specular"] && data["specular"].IsSequence()) {
        specular = Vec3(data["specular"][0].as<float>(), data["specular"][1].as<float>(),
                        data["specular"][2].as<float>());
    }

    float shininess = base.getShininess();
    if (data["shininess"])
        shininess = data["shininess"].as<float>();

    float dissolve = base.getDissolve();
    if (data["dissolve"])
        dissolve = data["dissolve"].as<float>();

    return Material(base.getDiffuseMap(), base.getSpecularMap(), ambient, diffuse, specular,
                    shininess, dissolve);
}
