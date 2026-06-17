#pragma once
#include "raylib.h"
#include "Player.h"
#include "EnemyGrid.h"

typedef struct {
    int width;
    int height;
} Screen;

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
    void UpdatePlaying();
    void DrawPlaying();

    Screen screen{800, 600};
    GameState state = GameState::MAIN_MENU;
    
    Player player;
    EnemyGrid enemyGrid;
    
    int score = 0;
    int lives = 3;
    int level = 1;
};
