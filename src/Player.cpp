#include "Player.h"
#include "Utils.h"
#include "BulletManager.h"

Player::Player() {
    rect = { GetScreenWidth() / 2.0f, GetScreenHeight() * 0.86f, PLAYER_WIDTH, PLAYER_HEIGHT };
}

Rectangle Player::GetRect() const {
    return rect;
}

void Player::Update(float deltaTime, BulletManager& bulletManager) {
    // Movement
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  rect.x -= speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) rect.x += speed * deltaTime;

    // Keep on screen
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > GetScreenWidth()) rect.x = GetScreenWidth() - rect.width;

    // Cooldown
    if (shootCooldown > 0) shootCooldown -= deltaTime;

    // Shooting
    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP)) && shootCooldown <= 0) {
        bulletManager.SpawnPlayerBullet({rect.x + rect.width/2, rect.y});
        shootCooldown = 0.25f;  // fire rate
    }
}

void Player::Draw() {
    // Draw the ship
    DrawShip(rect.x, rect.y);
}

void Player::DrawShip(float x, float y) {
    DrawRectangleRec({x, y+4, PLAYER_WIDTH, PLAYER_HEIGHT-4}, CYAN); // base
    Rectangle top = {x, y - 3, PLAYER_WIDTH, PLAYER_HEIGHT};
    DrawRectangleRounded(top, 0.7f, 5, CYAN); // top
    DrawTriangle({x + PLAYER_WIDTH/2, y - 9}, 
                 {x, y + PLAYER_HEIGHT}, 
                 {x + PLAYER_WIDTH, y + PLAYER_HEIGHT}, CYAN); // cockpit
}
