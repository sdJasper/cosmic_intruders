#pragma once
#include "raylib.h"

struct Enemy {
    Vector2 position;
    int width = 40;
    int height = 30;
    bool alive = true;
    int type = 0;           // 0 = top row, 1 = middle, 2 = bottom (for scoring / visuals later)
    float animationTimer = 0.0f;
};
