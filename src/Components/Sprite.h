#pragma once
#include "raylib.h"
#include "../Core/Component.h"

struct Sprite : public Component {
    Color color = WHITE;
    int width = 40;
    int height = 30;
    
    // For later when we add real images
    // Texture2D texture;
    
    Sprite(Color c = WHITE, int w = 40, int h = 30)
        : color(c), width(w), height(h) {}
};
