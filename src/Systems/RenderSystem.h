#pragma once
#include "../Core/World.h"

class RenderSystem {
public:
    RenderSystem(World& world);

    void Draw();

private:
    World& world;
};
