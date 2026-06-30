#include "EnemyGrid.h"
#include "Utils.h"
#include "BulletManager.h"

EnemyGrid::EnemyGrid() {
}

void EnemyGrid::Reset() {
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
            e.type = (row+1) / 2;                    // Higher rows = different enemy type
            e.alive = true;
            e.canShoot = canShoot;
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
            moveTimer = 0.0f;
            e.position.x += direction.x * speed * moveInterval;
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
    for (const auto& e : enemies) {
        if (!e.alive) continue;

        std::vector<Color> colors = { GREEN, CYAN, PURPLE };
        Color color = colors[e.type];

        DrawRectangle(e.position.x, e.position.y, e.width, e.height, color);
        DrawRectangleLines(e.position.x, e.position.y, e.width, e.height, BLACK);
    }
}

int EnemyGrid::GetAliveCount() const {
    int count = 0;
    for (const auto& e : enemies) {
        if (e.alive) count++;
    }
    return count;
}
