#include "EnemyGrid.h"
#include "Utils.h"
#include "BulletManager.h"

EnemyGrid::EnemyGrid() {
    bulletManager = BulletManager();
    Reset();
}

void EnemyGrid::Reset() {
    enemies.clear();
    
    const int cols = 11;
    const int rows = 5;
    const int startX = 90;
    const int startY = GetScreenHeight() * 0.25f;
    const int spacingX = ENEMY_WIDTH + 8;
    const int spacingY = ENEMY_HEIGHT + 16;

    for (int row = 0; row < rows; row++) {
        bool canShoot = (row == rows - 1); // Only the bottom row can shoot
        for (int col = 0; col < cols; col++) {
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

void EnemyGrid::Update(float deltaTime) {
    if (enemies.empty()) return;

    moveTimer += deltaTime;
    shootTimer += deltaTime;
    
    std::vector<size_t> canShootIndices;

    if (moveTimer < moveInterval) return;

    moveTimer = 0.0f;
    bool shouldDrop = false;

    // Move the whole grid sideways
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy& e = enemies[i];

        if (!e.alive) continue;

        e.position.x += direction.x * speed * moveInterval;

        if (direction.x > 0 && e.position.x + e.width > GetScreenWidth()-(e.width*2)) shouldDrop = true;
        if (direction.x < 0 && e.position.x < (e.width*2)) shouldDrop = true;

        if (e.canShoot) {
            canShootIndices.push_back(i);
        }
    }

    if (shouldDrop) {
        direction.x = -direction.x;
        for (auto& e : enemies) {
            if (e.alive) {
                e.position.y += dropDistance;
            }
        }
    }

    if (!canShootIndices.empty() && shootTimer >= shootInterval) {
        shootTimer = 0.0f;
        Enemy& shooter = enemies[canShootIndices[GetRandomValue(0, canShootIndices.size() - 1)]];
        bulletManager.SpawnEnemyBullet(shooter.position);
    }

    // Update bullets
    for (auto& b : bullets) {
        if (b.active) {
            b.position.y += b.velocity.y * deltaTime;
            if (b.position.y < 0) b.active = false;
        }
    }

}

void EnemyGrid::CheckHit(std::vector<Bullet>& playerBullets) {
    const int COLS = 11;

    for (auto& b : playerBullets) {
        if (!b.active) continue;

        for (size_t i = 0; i < enemies.size(); ++i) {
            Enemy& e = enemies[i];
            if (!e.alive) continue;

            Rectangle enemyRect = { e.position.x, e.position.y, 
                                  (float)e.width, (float)e.height };

            if (CheckCollisionCircleRec(b.position, b.radius, enemyRect)) {
                e.alive = false;
                e.canShoot = false;
                b.active = false;

                // === PROMOTE NEW LOWEST SHOOTER IN THE SAME COLUMN ===
                int col = i % COLS;

                // Scan this column from the bottom up, find the first (lowest) alive enemy
                for (int row = 4; row >= 0; --row) {           // 5 rows, bottom to top
                    int index = row * COLS + col;
                    if (index < (int)enemies.size() && enemies[index].alive) {
                        enemies[index].canShoot = true;
                        break;
                    }
                }

                speed += 2.0f;
                break;  // one collision per bullet
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

    // === DRAW BULLETS ===
    for (const auto& b : bullets) {
        if (b.active) {
            DrawLineEx(b.position, {b.position.x, b.position.y + 10}, 2, YELLOW);
        }
    }

}

int EnemyGrid::GetAliveCount() const {
    int count = 0;
    for (const auto& e : enemies) {
        if (e.alive) count++;
    }
    return count;
}
