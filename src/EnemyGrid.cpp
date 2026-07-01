#include "EnemyGrid.h"
#include "Utils.h"
#include "BulletManager.h"
#include <string>
#include "Shield.h"

EnemyGrid::EnemyGrid() {
}

void EnemyGrid::Reset(int level) {
    enemies.clear();
    moveInterval = std::max(BASE_INTERVAL - (level - 1) * 0.05f, MIN_INTERVAL);

    const int rows = 5;
    const int startX = 90;

    const int levelOffset = std::min((level -1) * 24, 128);
    const int startY = GetScreenHeight() * 0.15f + levelOffset; // 115, 123, 131

    const int spacingX = ENEMY_WIDTH + 8;
    const int spacingY = ENEMY_HEIGHT + 16;

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

bool EnemyGrid::Update(float deltaTime, BulletManager& bulletManager, std::vector<Shield>& shields, Sound& stepSound) {
    if (enemies.empty()) return false;

    moveTimer += deltaTime;
    shootTimer += deltaTime;
    std::vector<size_t> canShootIndices;

    if (moveTimer < moveInterval) return false;
    moveTimer = 0.0f;

    SetSoundPitch(stepSound, PITCHES[stepIndex % 4]);
    stepIndex++;
    PlaySound(stepSound);

    bool shouldDrop = false;

    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& e = enemies[i];
        if (!e.alive) continue;

        e.currentFrame = 1 - e.currentFrame;
        e.position.x += direction.x * STEP_SIZE;

        if (direction.x > 0 && e.position.x + e.width > GetScreenWidth() - (e.width * 1)) shouldDrop = true;
        if (direction.x < 0 && e.position.x < e.width * 2) shouldDrop = true;

        if (e.canShoot) canShootIndices.push_back(i);

        // erase any shield cells this enemy overlaps
        Rectangle enemyRect = { e.position.x, e.position.y, (float)e.width, (float)e.height };
        for (auto& shield : shields) {
            shield.EraseOverlap(enemyRect);
        }
    }

    if (shouldDrop) {
        direction.x = -direction.x;
        for (auto& e : enemies) {
            if (e.alive) e.position.y += dropDistance;
        }
        // check if lowest enemy has reached the player area
        float lowestY = 0.0f;
        for (const auto& e : enemies) {
            if (e.alive && e.position.y > lowestY) lowestY = e.position.y;
        }
        if (lowestY + ENEMY_HEIGHT >= GetScreenHeight() * 0.93f) {
            return true; // player hit
        }

    }

    if (!canShootIndices.empty() && shootTimer >= shootInterval) {
        shootTimer = 0.0f;
        Enemy& shooter = enemies[canShootIndices[GetRandomValue(0, canShootIndices.size() - 1)]];
        const Vector2 origin = { shooter.position.x + (shooter.width / 2), shooter.position.y + shooter.height };
        bulletManager.SpawnEnemyBullet(origin);
    }
    return false;
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