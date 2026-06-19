#pragma once
#include "raylib.h"
#include "Bullet.h"
#include <vector>

inline constexpr int PLAYER_WIDTH = 38;
inline constexpr int PLAYER_HEIGHT = 10;

class Player {
public:
    Rectangle rect;
    float speed = 400.0f;
    std::vector<Bullet> bullets;
    float shootCooldown = 0.0f;

    Player();
    void Update(float deltaTime);
    void Draw();
    void Shoot();
    static void DrawShip(float x, float y);
};
