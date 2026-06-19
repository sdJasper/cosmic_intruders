#pragma once
#include "raylib.h"
#include "../Core/Component.h"

struct Transform2D : public Component {
    Vector2 position;
    float rotation = 0.0f;
    float scale = 1.0f;

    Transform2D(float x = 0.0f, float y = 0.0f)
        : position{ x, y } {}
};
