#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

class RenderSystem; // Forward declaration

class World {
public:
    World();
    ~World();

    Entity* CreateEntity();
    void DestroyEntity(size_t id);
    
    void Update(float deltaTime);
    void Draw();

    std::vector<std::unique_ptr<Entity>>& GetEntities() { return entities; }

private:
    std::vector<std::unique_ptr<Entity>> entities;
    size_t nextID = 0;

    RenderSystem* renderSystem = nullptr;
};
