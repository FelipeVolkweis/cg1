#ifndef RENDERABLE_H
#define RENDERABLE_H

// Probably will define this later
class Renderable {
public:
    virtual ~Renderable() = 0;
    virtual void initializeOnGPU() = 0;
};

#endif