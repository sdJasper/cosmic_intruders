#pragma once
#include "raylib.h"
#include "World.h"
#include "Screen.h"

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
public:
    Game();
    void Run();

private:
    void Update();
    void Draw();

    Screen screen{672, 768}; // original was 224x256    x3 would be 672 x 768
    // Screen screen{800, 600};
    GameState state = GameState::MAIN_MENU;
    
    World world;
    
    int score = 0;
    int lives = 3;
    int level = 1;
};
