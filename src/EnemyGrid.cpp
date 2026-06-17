#include "EnemyGrid.h"

EnemyGrid::EnemyGrid() {
    Reset();
}

void EnemyGrid::Reset() {
    enemies.clear();
    
    const int cols = 11;
    const int rows = 5;
    const int startX = 90;
    const int startY = 80;
    const int spacingX = 55;
    const int spacingY = 45;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Enemy e;
            e.position.x = startX + col * spacingX;
            e.position.y = startY + row * spacingY;
            e.type = row;                    // Higher rows = different enemy type
            e.alive = true;
            enemies.push_back(e);
        }
    }
}

void EnemyGrid::Update(float deltaTime) {
    moveTimer += deltaTime;

    if (moveTimer >= moveInterval) {
        moveTimer = 0.0f;

        // Move the whole grid sideways
        for (auto& e : enemies) {
            if (e.alive) {
                e.position.x += direction.x * speed * moveInterval;
            }
        }

        // Check if we hit screen edges
        bool shouldDrop = false;
        for (const auto& e : enemies) {
            if (!e.alive) continue;
            if (direction.x > 0 && e.position.x + e.width > 780) shouldDrop = true;
            if (direction.x < 0 && e.position.x < 20) shouldDrop = true;
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
}

void EnemyGrid::CheckHit(std::vector<Bullet>& bullets) {
    for (auto& b : bullets) {
        if (!b.active) continue;
        for (auto& e : enemies) {
            if (!e.alive) continue;
            const Rectangle enemyRect = { e.position.x, e.position.y, e.width, e.height };
            if (CheckCollisionCircleRec(b.position, b.radius, enemyRect)) {
                e.alive = false;
                b.active = false;
                break;
            }
        }
    }
}

void EnemyGrid::Draw() {
    for (const auto& e : enemies) {
        if (!e.alive) continue;

        Color color = (e.type <= 1) ? RED : 
                     (e.type == 2) ? ORANGE : YELLOW;

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
