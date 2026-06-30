#pragma once
#include "raylib.h"

inline constexpr int ENEMY_WIDTH = 33;
inline constexpr int ENEMY_HEIGHT = 24;

struct Enemy {
    Vector2 position;
    int width = ENEMY_WIDTH;
    int height = ENEMY_HEIGHT;
    bool alive = true;
    int type = 0;
    float animationTimer = 0.0f;
    int currentFrame = 0;
    bool canShoot = false;

    Texture2D texture[2];
};
