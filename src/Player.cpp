#include "Player.h"

Player::Player() {
    rect = { 400 - 25, 500, 50, 30 };
}

void Player::Update(float deltaTime) {
    // Movement
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  rect.x -= speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) rect.x += speed * deltaTime;

    // Keep on screen
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > 800) rect.x = 800 - rect.width;

    // Cooldown
    if (shootCooldown > 0) shootCooldown -= deltaTime;

    // Shooting
    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP)) && shootCooldown <= 0) {
        Shoot();
        shootCooldown = 0.25f;  // fire rate
    }

    // Update bullets
    for (auto& b : bullets) {
        if (b.active) {
            b.position.y += b.velocity.y * deltaTime;
            if (b.position.y < 0) b.active = false;
        }
    }
}

void Player::Shoot() {
    for (auto& b : bullets) {
        if (!b.active) {
            b.position = { rect.x + rect.width/2, rect.y };
            b.velocity = { 0, -600 };
            b.active = true;
            return;
        }
    }
    
    // Add new bullet if none available
    bullets.push_back({ {rect.x + rect.width/2, rect.y}, {0, -600}, true });
}

void Player::Draw() {
    // Draw the ship
    DrawRectangleRec(rect, WHITE);
    DrawTriangle({rect.x + rect.width/2, rect.y - 8}, 
                 {rect.x, rect.y + rect.height}, 
                 {rect.x + rect.width, rect.y + rect.height}, WHITE);

    // === DRAW BULLETS ===
    for (const auto& b : bullets) {
        if (b.active) {
            DrawCircleV(b.position, b.radius, YELLOW);
            // Optional: Make bullets look nicer
            // DrawCircleV(b.position, b.radius + 1, Fade(YELLOW, 0.3f));
        }
    }
}
