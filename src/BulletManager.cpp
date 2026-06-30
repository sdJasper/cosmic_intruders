#include "BulletManager.h"
#include "Enemy.h"
#include "EnemyGrid.h"
#include "Player.h"

void BulletManager::SpawnPlayerBullet(Vector2 position) {
    for (auto& b : playerBullets) {
        if (!b.active) {
            b.position = position;
            b.velocity = { 0.0f, -720.0f };
            b.active = true;
            b.radius = 4.0f;
            b.color = YELLOW;
            return;
        }
    }

    // Create new if no inactive bullet found
    Bullet b;
    b.position = position;
    b.velocity = { 0.0f, -720.0f };
    b.active = true;
    b.radius = 4.0f;
    b.color = YELLOW;
    playerBullets.push_back(b);
}

void BulletManager::SpawnEnemyBullet(Vector2 position) {
    for (auto& b : enemyBullets) {
        if (!b.active) {
            b.position = position;
            b.velocity = { 0.0f, 210.0f };    // downward
            b.active = true;
            b.radius = 4.0f;
            b.color = RED;
            return;
        }
    }

    Bullet b;
    b.position = position;
    b.velocity = { 0.0f, 210.0f };
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

int BulletManager::CheckCollisions(EnemyGrid& enemyGrid, Player& player, bool& playerHit) {
    auto& enemies = enemyGrid.GetEnemies(); // calls the non-const overload
    const auto& playerRect = player.GetRect();
    int score = 0;
    playerHit = false;

    // Player bullets vs Enemies
    for (auto& pb : playerBullets) {
        if (!pb.active) continue;

        for (size_t i = 0; i < enemies.size(); ++i) {
            const Enemy& e = enemies[i];
            if (!e.alive) continue;

            Rectangle enemyRect = { e.position.x, e.position.y, 
                                  (float)e.width, (float)e.height };

            if (CheckCollisionCircleRec(pb.position, pb.radius, enemyRect)) {
                // Hit!
                // Note: Since 'enemies' is const, we need to modify via enemyGrid
                Enemy& mutableEnemy = const_cast<Enemy&>(e);  // ugly but works for now
                mutableEnemy.alive = false;
                mutableEnemy.canShoot = false;
                pb.active = false;
                score += 30 - (e.type * 10);
                // Promote new shooter in column
                int col = i % COLS;
                for (int row = 4; row >= 0; --row) {
                    int index = row * COLS + col;
                    if (index < (int)enemies.size() && enemies[index].alive) {
                        Enemy& newShooter = const_cast<Enemy&>(enemies[index]);
                        newShooter.canShoot = true;
                        break;
                    }
                }

                break;
            }
        }
    }

    // Check collisions between enemy bullets and the player

    for (auto& eb : enemyBullets) {
        if (!eb.active) continue;

        if (CheckCollisionCircleRec(eb.position, eb.radius, playerRect)) {
            // Handle player hit
            eb.active = false;
            playerHit = true;
            break;  // one collision per bullet
        }
    }

    return score;
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

        // DrawCircleV(b.position, b.radius, b.color);
        DrawLine(b.position.x, b.position.y, b.position.x, b.position.y + 10, b.color);
    }
}