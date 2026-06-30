#include "BulletManager.h"
#include "Enemy.h"
#include "EnemyGrid.h"
#include "Player.h"
#include "Shield.h"

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

int BulletManager::CheckCollisions(EnemyGrid& enemyGrid, Player& player, std::vector<Shield>& shields, bool& playerHit) {
    int pointsEarned = 0;
    playerHit = false;

    // --- Player bullets vs shields ---
    for (auto& pb : playerBullets) {
        if (!pb.active) continue;
        for (auto& shield : shields) {
            if (shield.CheckHit(pb.position, pb.radius)) {
                pb.active = false;
                break;
            }
        }
    }

    // --- Enemy bullets vs shields ---
    for (auto& eb : enemyBullets) {
        if (!eb.active) continue;
        for (auto& shield : shields) {
            if (shield.CheckHit(eb.position, eb.radius)) {
                eb.active = false;
                break;
            }
        }
    }

    // --- Player bullets vs enemies ---
    auto& enemies = enemyGrid.GetEnemies();
    for (auto& pb : playerBullets) {
        if (!pb.active) continue;

        for (auto& enemy : enemies) {
            if (!enemy.alive) continue;

            Rectangle enemyRect = { enemy.position.x, enemy.position.y, enemy.width, enemy.height };
            if (CheckCollisionCircleRec(pb.position, pb.radius, enemyRect)) {
                enemy.alive = false;
                enemy.canShoot = false;
                pb.active = false;
                pointsEarned += 10;
                BulletManager::DrawExplosion({enemy.position.x + enemy.width / 2, enemy.position.y + enemy.height / 2}, YELLOW);

                int col = (&enemy - &enemies[0]) % COLS;
                for (int row = 0; row < (int)enemies.size() / COLS; row++) {
                    int idx = row * COLS + col;
                    if (idx < (int)enemies.size() && enemies[idx].alive) {
                        enemies[idx].canShoot = true;
                        break;
                    }
                }
                break;
            }
        }
    }

    // --- Enemy bullets vs player ---
    for (auto& eb : enemyBullets) {
        if (!eb.active) continue;

        Rectangle playerRect = { player.rect.x, player.rect.y, player.rect.width, player.rect.height };
        if (CheckCollisionCircleRec(eb.position, eb.radius, playerRect)) {
            eb.active = false;
            playerHit = true;
        }
    }

    return pointsEarned;
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

void BulletManager::DrawExplosion(Vector2 center, Color color) {
    float size = 8.0f;
    DrawLineEx(center, {center.x + size, center.y}, 4, color);
    DrawLineEx(center, {center.x - size, center.y}, 4, color);
    DrawLineEx(center, {center.x, center.y + size}, 4, color);
    DrawLineEx(center, {center.x, center.y - size}, 4, color);

    float d = size * 0.707f; // ~45 degrees
    DrawLineEx(center, {center.x + d, center.y + d}, 3, YELLOW);
    DrawLineEx(center, {center.x - d, center.y - d}, 3, YELLOW);
    DrawLineEx(center, {center.x + d, center.y - d}, 2.5f, Fade(RED, 0.7f));
    DrawLineEx(center, {center.x - d, center.y + d}, 2.5f, Fade(RED, 0.7f));

}

