#ifndef RENDERABLELIGHT_H
#define RENDERABLELIGHT_H

#include <memory>

#include "camera/camera.h"
#include "directionallight.h"
#include "pointlight.h"
#include "renderer/renderable.h"
#include "renderer/shader/shader.h"
#include "renderer/shadows/shadow.h"
#include "spotlight.h"

// !!SYNC THIS WITH THE RENDERER.FRAG SHADER!!
#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 16

class RenderableLight : public Renderable {
public:
    virtual void setLight(std::shared_ptr<BaseLight> light) = 0;

    void setShaderProgram(std::shared_ptr<Shader> shaderProgram) {
        shaderProgram_ = shaderProgram;
    }

    bool hasShaderProgram() {
        return shaderProgram_ != nullptr;
    }

    void setFrameBufferAspect(float frameBufferAspect) {
        frameBufferAspect_ = frameBufferAspect;
    }

    void setCamera(std::shared_ptr<Camera> camera) {
        camera_ = camera;
    }
    
    std::shared_ptr<Camera> camera_;
    float frameBufferAspect_;
protected:
    std::shared_ptr<Shader> shaderProgram_ = nullptr;
};

class RenderableDirectionalLight : public RenderableLight {
public:
    RenderableDirectionalLight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;
    void updateShadows();

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<DirectionalLight>(light);
    }
    
    Shadow& getShadow() { return shadow_; }

private:
    std::shared_ptr<DirectionalLight> light_;
    uint64_t id_ = -1;

    Shadow shadow_;
    uint32_t ubo_;
};

class RenderablePointLight : public RenderableLight {
public:
    RenderablePointLight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;

    void setIndex(int index) {
        index_ = index;
    }

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<PointLight>(light);
    }

private:
    std::shared_ptr<PointLight> light_;
    uint64_t id_ = -1;
    int index_;
};

class RenderableSpotlight : public RenderableLight {
public:
    RenderableSpotlight(uint64_t id);

    void initializeOnGPU() override;
    void render() override;

    void setIndex(int index) {
        index_ = index;
    }

    void setLight(std::shared_ptr<BaseLight> light) override {
        light_ = std::static_pointer_cast<Spotlight>(light);
    }

private:
    std::shared_ptr<Spotlight> light_;
    uint64_t id_ = -1;
    int index_;
};

#endif