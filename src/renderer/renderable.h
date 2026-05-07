#ifndef RENDERABLE_H
#define RENDERABLE_H

// Probably will define this later
class Renderable {
public:
    virtual void initializeOnGPU() = 0;
    virtual void render() = 0;
};

#endif