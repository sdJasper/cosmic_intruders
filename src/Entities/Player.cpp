#include "Player.h"

Player::Player(World& worldRef) : world(worldRef) {
    // Create the entity in the world
    entity = world.CreateEntity();

    // Add components
    entity->AddComponent<Transform2D>(400.0f, 520.0f);           // Starting position
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

    auto* transform2d = entity->GetComponent<Transform2D>();
    auto* velocity = entity->GetComponent<Velocity>();

    if (!transform2d || !velocity) return;

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
    transform2d->position.x += velocity->value.x * deltaTime;

    // Keep player within bounds
    if (transform2d->position.x < leftBoundary)
        transform2d->position.x = leftBoundary;
    
    if (transform2d->position.x > rightBoundary - 48)        // 48 = ship width
        transform2d->position.x = rightBoundary - 48;
}

void Player::Draw() {
    if (!entity || !entity->alive) return;

    auto* transform2d = entity->GetComponent<Transform2D>();
    auto* velocity = entity->GetComponent<Velocity>();
    auto* sprite   = entity->GetComponent<Sprite>();

    if (transform2d && sprite) {
        // Draw ship body
        DrawRectangleRec(
            Rectangle{transform2d->position.x, transform2d->position.y, (float)sprite->width, (float)sprite->height},
            sprite->color
        );

        // Draw simple ship "nose" / triangle on top
        DrawTriangle(
            {transform2d->position.x + sprite->width/2, transform2d->position.y - 8},
            {transform2d->position.x, transform2d->position.y + sprite->height},
            {transform2d->position.x + sprite->width, transform2d->position.y + sprite->height},
            WHITE
        );
    }
}
