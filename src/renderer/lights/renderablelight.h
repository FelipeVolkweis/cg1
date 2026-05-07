#ifndef RENDERABLELIGHT_H
#define RENDERABLELIGHT_H

#include <memory>

#include "directionallight.h"
#include "pointlight.h"
#include "renderer/renderable.h"
#include "spotlight.h"

// !!SYNC THIS WITH THE RENDERER.FRAG SHADER!!
#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 16

class RenderableLight : public Renderable {
public:
    virtual void setLight(std::shared_ptr<BaseLight> light) = 0;
};

class RenderableDirectionalLight : public RenderableLight {
public:
    RenderableDirectionalLight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;

    void setShaderProgram(uint32_t shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<DirectionalLight>(light);
    }

private:
    std::shared_ptr<DirectionalLight> light_;

    uint32_t shaderProgram_ = 0;
    uint64_t id_ = -1;

    struct DirectionalLightLocations {
        int direction;
        int ambient;
        int diffuse;
        int specular;
    };

    DirectionalLightLocations locations_;
};

class RenderablePointLight : public RenderableLight {
public:
    RenderablePointLight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;

    void setShaderProgram(uint32_t shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

    void setIndex(int index) {
        index_ = index;
    }

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<PointLight>(light);
    }

private:
    std::shared_ptr<PointLight> light_;

    uint32_t shaderProgram_ = 0;
    uint64_t id_ = -1;
    int index_;

    struct PointLightLocations {
        int position;
        int constant;
        int linear;
        int quadratic;
        int ambient;
        int diffuse;
        int specular;
    };

    PointLightLocations locations_;
};

class RenderableSpotlight : public RenderableLight {
public:
    RenderableSpotlight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;

    void setShaderProgram(uint32_t shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

    void setIndex(int index) {
        index_ = index;
    }

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<Spotlight>(light);
    }

private:
    std::shared_ptr<Spotlight> light_;

    uint32_t shaderProgram_ = 0;
    uint64_t id_ = -1;
    int index_;

    struct SpotlightLocations {
        int position;
        int direction;
        int cutoff;
        int outerCutoff;
        int constant;
        int linear;
        int quadratic;
        int ambient;
        int diffuse;
        int specular;
    };

    SpotlightLocations locations_;
};
#endif