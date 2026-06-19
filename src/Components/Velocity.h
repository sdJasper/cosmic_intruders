#pragma once
#include "raylib.h"
#include "../Core/Component.h"

struct Velocity : public Component {
    Vector2 value;        // direction and speed

    Velocity(float x = 0.0f, float y = 0.0f)
        : value{ x, y } {}
};
