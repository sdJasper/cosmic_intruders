#pragma once
#include "raylib.h"
#include "Bullet.h"
#include <vector>
#include "BulletManager.h"

inline constexpr int PLAYER_WIDTH = 38;
inline constexpr int PLAYER_HEIGHT = 10;

class Player {
public:
    Rectangle rect;
    float speed = 400.0f;
    std::vector<Bullet> bullets;
    float shootCooldown = 0.0f;
    BulletManager bulletManager;

    Player();
    void Update(float deltaTime);
    void Draw();
    static void DrawShip(float x, float y);
};
