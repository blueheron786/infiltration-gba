#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Component.h"

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;
    
    // Delete copy constructor and copy assignment operator
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    
    // Allow move constructor and move assignment operator
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    // Add a component to the entity
    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Get a component from the entity
    template<typename T>
    T* getComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    // Get a component from the entity (const version)
    template<typename T>
    const T* getComponent() const {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return static_cast<const T*>(it->second.get());
        }
        return nullptr;
    }

protected:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};
