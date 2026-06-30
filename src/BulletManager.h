#pragma once
#include "Bullet.h"
#include <vector>

class Player;
class EnemyGrid;

class BulletManager {
public:
    void SpawnPlayerBullet(Vector2 position);
    void SpawnEnemyBullet(Vector2 position);

    void Update(float deltaTime);
    void Draw();

    int CheckCollisions(EnemyGrid& enemyGrid, Player& player, bool& playerHit);

    void Reset();

private:
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;

    void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime);
    void DrawBullets(const std::vector<Bullet>& bullets);
};