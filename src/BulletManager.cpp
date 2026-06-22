#include "BulletManager.h"

// BulletManager::BulletManager() {
// 

void BulletManager::SpawnPlayerBullet(Vector2 position) {
    for (auto& b : playerBullets) {
        if (!b.active) {
            b.position = position;
            b.velocity = { 0.0f, -620.0f };   // upward
            b.active = true;
            b.radius = 4.0f;
            b.color = YELLOW;                 // or WHITE
            return;
        }
    }

    // Create new if no inactive bullet found
    Bullet b;
    b.position = position;
    b.velocity = { 0.0f, -620.0f };
    b.active = true;
    b.radius = 4.0f;
    b.color = YELLOW;
    playerBullets.push_back(b);
}

void BulletManager::SpawnEnemyBullet(Vector2 position) {
    for (auto& b : enemyBullets) {
        if (!b.active) {
            b.position = position;
            b.velocity = { 0.0f, 480.0f };    // downward
            b.active = true;
            b.radius = 4.0f;
            b.color = RED;
            return;
        }
    }

    Bullet b;
    b.position = position;
    b.velocity = { 0.0f, 480.0f };
    b.active = true;
    b.radius = 4.0f;
    b.color = RED;
    enemyBullets.push_back(b);
}

void BulletManager::Update(float deltaTime) {
    UpdateBullets(playerBullets, deltaTime);
    UpdateBullets(enemyBullets, deltaTime);
}

void BulletManager::Draw() {
    DrawBullets(playerBullets);
    DrawBullets(enemyBullets);
}

void BulletManager::CheckCollisions(EnemyGrid& enemyGrid, Player& player) {
    // Implementation for collision detection
}

void BulletManager::Reset() {
    playerBullets.clear();
    enemyBullets.clear();
}

void BulletManager::UpdateBullets(std::vector<Bullet>& bullets, float deltaTime) {
    for (auto& b : bullets) {
        if (!b.active) continue;

        b.position.x += b.velocity.x * deltaTime;
        b.position.y += b.velocity.y * deltaTime;

        // Deactivate if off screen
        if (b.position.y < -20 || b.position.y > GetScreenHeight() + 20) {
            b.active = false;
        }
    }
}

void BulletManager::DrawBullets(const std::vector<Bullet>& bullets) {
    for (const auto& b : bullets) {
        if (!b.active) continue;

        DrawCircleV(b.position, b.radius, b.color);
    }
}