#pragma once
#include "Enemy.h"
#include <vector>
#include "Bullet.h"

class EnemyGrid {
public:
    std::vector<Enemy> enemies;
    Vector2 direction = { 1.0f, 0.0f };
    float speed = 90.0f;
    float moveTimer = 0.0f;
    float moveInterval = 0.75f;
    float dropDistance = 35.0f;

    EnemyGrid();
    void Update(float deltaTime);
    void Draw();
    void Reset();
    int GetAliveCount() const;
    void CheckHit(std::vector<Bullet>& bullets);
};
