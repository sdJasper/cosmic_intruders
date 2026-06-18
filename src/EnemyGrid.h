#pragma once
#include "Enemy.h"
#include <vector>
#include "Bullet.h"

class EnemyGrid {
public:
    std::vector<Enemy> enemies;
    Vector2 direction = { 1.0f, 0.0f };
    float speed = 5.0f;
    float moveTimer = 0.0f;
    float moveInterval = 0.2f;
    float shootTimer = 0.0f;
    float shootInterval = 1.0f;
    float dropDistance = 35.0f;
    std::vector<Bullet> bullets;

    EnemyGrid();
    void Update(float deltaTime);
    void Draw();
    void Reset();
    int GetAliveCount() const;
    void Shoot(const std::vector<Enemy>& enemies);
    void CheckHit(std::vector<Bullet>& bullets);
};
