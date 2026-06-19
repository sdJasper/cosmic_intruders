#pragma once
#include "Component.h"
#include <vector>
#include <memory>

class Entity {
public:
    size_t id;
    bool alive = true;

    Entity(size_t id) : id(id) {}

    // Add a component to the entity
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        components.push_back(std::move(component));
        return ptr;
    }

    // Get a component
    template<typename T>
    T* GetComponent() {
        for (auto& comp : components) {
            if (T* ptr = dynamic_cast<T*>(comp.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<Component>> components;
};
