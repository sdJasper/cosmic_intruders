#pragma once
#include "../Core/Entity.h"
#include "../Components/Transform2D.h"
#include "../Components/Velocity.h"
#include "../Components/Sprite.h"
#include "../Core/World.h"

class Player {
public:
    Player(World& world);
    ~Player();

    void Update(float deltaTime);
    void Draw();

private:
    Entity* entity = nullptr;
    World& world;

    float moveSpeed = 380.0f;
    float leftBoundary = 20.0f; // maybe make this a percent later?
    float rightBoundary = 780.0f; // maybe make this a percent later?
};
