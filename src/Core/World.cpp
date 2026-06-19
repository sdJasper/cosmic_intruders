#include "World.h"
#include "../Systems/RenderSystem.h"
#include <algorithm>

World::World() {
    entities.reserve(200);
    renderSystem = new RenderSystem(*this);
}

World::~World() {
    delete renderSystem;
}

Entity* World::CreateEntity() {
    auto entity = std::make_unique<Entity>(nextID++);
    Entity* ptr = entity.get();
    entities.push_back(std::move(entity));
    return ptr;
}

void World::DestroyEntity(size_t id) {
    for (auto& e : entities) {
        if (e->id == id) {
            e->alive = false;
            return;
        }
    }
}

void World::Update(float deltaTime) {
    // Remove dead entities
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const auto& e) { return !e->alive; }), entities.end());

    // TODO: Later we will call Systems here (Movement, AI, Collision, etc.)
}

void World::Draw() {
    if (renderSystem) renderSystem->Draw();
}
