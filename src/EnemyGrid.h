#pragma once
#include "Enemy.h"
#include <vector>
#include "Bullet.h"

class BulletManager;

class EnemyGrid {
public:
    std::vector<Enemy> enemies;
    Vector2 direction = { 1.0f, 0.0f };
    float speed = 6.0f;
    float moveTimer = 0.0f;
    float moveInterval = 0.8f;
    float shootTimer = 0.0f;
    float shootInterval = 1.0f;
    float dropDistance = 35.0f;
    std::vector<Bullet> bullets;
    const std::vector<Enemy>& GetEnemies() const { return enemies; }
    std::vector<Enemy>& GetEnemies() { return enemies; }

    EnemyGrid();
    void Update(float deltaTime, BulletManager& bulletManager);
    void Draw();
    void Reset();
    int GetAliveCount() const;
    void CheckHit(std::vector<Bullet>& bullets);
    Enemy* GetEnemyAt(int row, int col);
};
