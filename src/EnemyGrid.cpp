#include "EnemyGrid.h"
#include "Utils.h"

EnemyGrid::EnemyGrid() {
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
    moveTimer += deltaTime;
    shootTimer += deltaTime;
    std::vector<Enemy> canShoot;;

    if (moveTimer >= moveInterval) {
        moveTimer = 0.0f;

        // Move the whole grid sideways
        // Check if we hit screen edges
        bool shouldDrop = false;
        for (auto& e : enemies) {
            if (!e.alive) continue;

            e.position.x += direction.x * speed * moveInterval;
            if (direction.x > 0 && e.position.x + e.width > GetScreenWidth()-(e.width*2)) shouldDrop = true;
            if (direction.x < 0 && e.position.x < (e.width*2)) shouldDrop = true;

            if (e.canShoot) {
                canShoot.push_back(e);
            }
        }

        if (shouldDrop) {
            direction.x *= -1.0f;   // Reverse direction

            // Drop down
            for (auto& e : enemies) {
                if (e.alive) {
                    e.position.y += dropDistance;
                }
            }
        }
    }
    if (shootTimer >= shootInterval) {
        shootTimer = 0.0f;

        if (!canShoot.empty()) {
            Shoot(canShoot);
            // Handle shooting logic for the enemies that can shoot
        }
    }

    // Update bullets
    for (auto& b : bullets) {
        if (b.active) {
            b.position.y += b.velocity.y * deltaTime;
            if (b.position.y < 0) b.active = false;
        }
    }

}

void EnemyGrid::Shoot(const std::vector<Enemy>& enemies) {
    Enemy e = enemies[GetRandomValue(0, enemies.size() - 1)]; // random enemy from the list
    Bullet b;
    b.position = { e.position.x + e.width/2, e.position.y + e.height };
    b.velocity = { 0, +600 };
    b.active = true;
    bullets.push_back(b);
}

void EnemyGrid::CheckHit(std::vector<Bullet>& bullets) {
    for (auto& b : bullets) {
        if (!b.active) continue;
        int enemyIndex = -1;
        for (auto& e : enemies) {
            enemyIndex++;

            if (!e.alive) continue;
            const Rectangle enemyRect = { e.position.x, e.position.y, e.width+0.0f, e.height+0.0f };

            if (CheckCollisionCircleRec(b.position, b.radius, enemyRect)) {
                e.alive = false;
                e.canShoot = false;
                for (int i = enemyIndex-11; i >= 0; i-=11) {
                    Enemy higherEnemy = enemies[i];
                    if (higherEnemy.alive) {
                        higherEnemy.canShoot = true;
                        break;
                    }
                }
                b.active = false;
                speed += 2.0f;
                break;
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
