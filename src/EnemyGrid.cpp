#include "EnemyGrid.h"
#include "Utils.h"
#include "BulletManager.h"
#include <string>

EnemyGrid::EnemyGrid() {
}

void EnemyGrid::Reset() {
    moveInterval = BASE_INTERVAL;

    const int rows = 5;
    const int startX = 90;
    const int startY = GetScreenHeight() * 0.25f;
    const int spacingX = ENEMY_WIDTH + 8;
    const int spacingY = ENEMY_HEIGHT + 16;
    enemies.clear();

    for (int row = 0; row < rows; row++) {
        bool canShoot = (row == rows - 1); // Only the bottom row can shoot
        for (int col = 0; col < COLS; col++) {
            Enemy e;
            e.position.x = startX + col * spacingX;
            e.position.y = startY + row * spacingY;
            e.type = (row+1) / 2; // Assign type based on row (0, 1, 2)
            e.alive = true;
            e.canShoot = canShoot;
            const std::string texturePath = "assets/enemy" + std::to_string(e.type);
            e.texture[0] = LoadTexture((texturePath + "_0.png").c_str());
            e.texture[1] = LoadTexture((texturePath + "_1.png").c_str());
            enemies.push_back(e);
        }
    }
}

void EnemyGrid::Update(float deltaTime, BulletManager& bulletManager) {
    if (enemies.empty()) return;

    moveTimer += deltaTime;
    shootTimer += deltaTime;
    std::vector<size_t> canShootIndices;

    bool shouldDrop = false;
    bool shouldMove = false;
    if (moveTimer >= moveInterval) {
        moveTimer = 0.0f;
        shouldMove = true;
    }

    // Load grid data
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& e = enemies[i];
        if (!e.alive) continue;
        if (e.canShoot) canShootIndices.push_back(i);

        if (shouldMove) {
            e.currentFrame = 1 - e.currentFrame;
            e.position.x += direction.x * STEP_SIZE;
            if (e.position.x + e.width > GetScreenWidth()-(e.width*1) || e.position.x < (e.width*2)) {
                shouldDrop = true;
            }
        }
    }

    if (!canShootIndices.empty() && shootTimer >= shootInterval) {
        shootTimer = 0.0f;
        Enemy& shooter = enemies[canShootIndices[GetRandomValue(0, canShootIndices.size() - 1)]];
        const Vector2 origin = {shooter.position.x + (shooter.width / 2), shooter.position.y + shooter.height};
        bulletManager.SpawnEnemyBullet(origin);
    }


    if (shouldDrop) {
        direction.x = -direction.x;
        for (auto& e : enemies) {
            if (e.alive) {
                e.position.y += dropDistance;
            }
        }
    }
}

void EnemyGrid::Draw() {
    const Color collorArray[] = {RED, ORANGE, YELLOW};
    for (auto& e : enemies) {
        if (!e.alive) continue;

        DrawTexture(e.texture[e.currentFrame], (int)e.position.x, (int)e.position.y, collorArray[e.type]);
    }
}

int EnemyGrid::GetAliveCount() const {
    int count = 0;
    for (const auto& e : enemies) {
        if (e.alive) count++;
    }
    return count;
}

void EnemyGrid::OnEnemyKilled() {
    int alive = GetAliveCount();
    if (alive <= 0) return;

    float t = 1.0f - ((float)alive / (float)TOTAL_ENEMIES); // 0.0 full, 1.0 last one
    moveInterval = BASE_INTERVAL + t * (MIN_INTERVAL - BASE_INTERVAL);
}