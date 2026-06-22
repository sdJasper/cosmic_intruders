#pragma once
#include "raylib.h"

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool active = false;
    float radius = 4.0f;
    Color color = YELLOW;
};
