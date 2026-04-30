#ifndef COMPONENT_H
#define COMPONENT_H

class BaseComponent {
public:
    virtual bool onStart() {
        return true;
    }

    virtual void onPreUpdate() {};
    virtual void onUpdate() {};
    virtual void onPostUpdate() {};

    virtual bool onEnd() {
        return true;
    }
};

#endif