#pragma once
#include "raylib.h"
#include "Bullet.h"
#include <vector>

class BulletManager;

inline constexpr int PLAYER_WIDTH = 38;
inline constexpr int PLAYER_HEIGHT = 10;

class Player {
public:
    Rectangle rect;
    float speed = 400.0f;
    float shootCooldown = 0.0f;
    Rectangle GetRect() const;

    Player();
    void Update(float deltaTime, BulletManager& bulletManager);
    void Draw();
    static void DrawShip(float x, float y);
};
