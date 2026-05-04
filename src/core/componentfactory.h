#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "component.h"

/**
 * @class ComponentFactory
 * @brief Factory class for creating components by their type name.
 * 
 * This class follows the Singleton pattern.
 */
class ComponentFactory {
public:
    using Creator = std::function<std::shared_ptr<BaseComponent>()>;

    /**
     * @brief Gets the singleton instance of the ComponentFactory.
     * @return A reference to the ComponentFactory instance.
     */
    static ComponentFactory &getInstance();

    /**
     * @brief Registers a component creator function for a specific type.
     * @param type The type name of the component.
     * @param creator The creator function.
     */
    void registerComponent(const std::string &type, Creator creator);

    /**
     * @brief Creates a component of a specific type.
     * @param type The type name of the component to create.
     * @return A shared pointer to the created component, or nullptr if the type is not registered.
     */
    std::shared_ptr<BaseComponent> createComponent(const std::string &type);

private:
    ComponentFactory();
    std::unordered_map<std::string, Creator> creators_;
};

#endif
