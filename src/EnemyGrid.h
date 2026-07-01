#pragma once
#include "Enemy.h"
#include <vector>
#include "Bullet.h"

class BulletManager;

inline constexpr int COLS = 11;

class EnemyGrid {
private:
    static constexpr float BASE_INTERVAL = 0.8f;
    static constexpr float MIN_INTERVAL = 0.05f;
    static constexpr int TOTAL_ENEMIES = 55;
    static constexpr int STEP_SIZE = 12.0f;

    Vector2 direction = { 1.0f, 0.0f };
    float speed = 6.0f;
    float moveTimer = 0.0f;
    float moveInterval = 0.8f;
    float shootTimer = 0.0f;
    float shootInterval = 1.0f;
    float dropDistance = 35.0f;
    std::vector<Enemy> enemies;

public:
    std::vector<Enemy>& GetEnemies() { return enemies; }

    EnemyGrid();
    void Update(float deltaTime, BulletManager& bulletManager);
    void Draw();
    void Reset(int level = 1);
    int GetAliveCount() const;
    void OnEnemyKilled();
};
