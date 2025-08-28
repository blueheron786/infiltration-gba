#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>

class Component; // Forward declaration

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

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

protected:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};
