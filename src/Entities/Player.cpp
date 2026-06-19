#include "Player.h"

Player::Player(World& worldRef) : world(worldRef) {
    // Create the entity in the world
    entity = world.CreateEntity();

    // Add components
    entity->AddComponent<Position>(400.0f, 520.0f);           // Starting position
    entity->AddComponent<Velocity>(0.0f, 0.0f);
    entity->AddComponent<Sprite>(WHITE, 48, 32);              // width, height
}

Player::~Player() {
    if (entity) {
        entity->alive = false;
    }
}

void Player::Update(float deltaTime) {
    if (!entity || !entity->alive) return;

    auto* position = entity->GetComponent<Position>();
    auto* velocity = entity->GetComponent<Velocity>();

    if (!position || !velocity) return;

    // Reset horizontal velocity
    velocity->value.x = 0.0f;

    // Input
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        velocity->value.x = -moveSpeed;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        velocity->value.x = moveSpeed;
    }

    // Apply movement
    position->position.x += velocity->value.x * deltaTime;

    // Keep player within bounds
    if (position->value.x < leftBoundary)
        position->value.x = leftBoundary;
    
    if (position->value.x > rightBoundary - 48)        // 48 = ship width
        position->value.x = rightBoundary - 48;
}

void Player::Draw() {
    if (!entity || !entity->alive) return;

    auto* position = entity->GetComponent<Position>();
    auto* sprite   = entity->GetComponent<Sprite>();

    if (position && sprite) {
        // Draw ship body
        DrawRectangleRec(
            Rectangle{position->value.x, position->value.y, (float)sprite->width, (float)sprite->height},
            sprite->color
        );

        // Draw simple ship "nose" / triangle on top
        DrawTriangle(
            {position->value.x + sprite->width/2, position->value.y - 8},
            {position->value.x, position->value.y + sprite->height},
            {position->value.x + sprite->width, position->value.y + sprite->height},
            WHITE
        );
    }
}
