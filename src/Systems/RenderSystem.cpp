#include "RenderSystem.h"
#include "../Components/Transform2D.h"
#include "../Components/Sprite.h"

RenderSystem::RenderSystem(World& worldRef) : world(worldRef) {}

void RenderSystem::Draw() {
    for (const auto& entityPtr : world.GetEntities()) {
        if (!entityPtr->alive) continue;

        Entity* e = entityPtr.get();

        auto* transform = e->GetComponent<Transform2D>();
        auto* sprite    = e->GetComponent<Sprite>();

        if (transform && sprite) {
            // Draw the entity as a rectangle for now
            DrawRectangle(
                (int)transform->position.x,
                (int)transform->position.y,
                sprite->width,
                sprite->height,
                sprite->color
            );
        }
    }
}
