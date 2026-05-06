#include "material.h"

Material::Material(uint32_t diffuseTextureId, uint32_t specularTextureId, float shininess,
                   float dissolve)
    : diffuseTextureId_(diffuseTextureId), specularTextureId_(specularTextureId),
      shininess_(shininess), dissolve_(dissolve) {}
