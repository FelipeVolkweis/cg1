#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "component.h"

class ComponentFactory {
public:
    using Creator = std::function<std::shared_ptr<BaseComponent>()>;

    static ComponentFactory &getInstance();

    void registerComponent(const std::string &type, Creator creator);
    std::shared_ptr<BaseComponent> createComponent(const std::string &type);

private:
    ComponentFactory();
    std::unordered_map<std::string, Creator> creators_;
};

#endif
