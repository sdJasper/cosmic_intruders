#pragma once
#include "raylib.h"
#include "Player.h"
#include "EnemyGrid.h"
#include "BulletManager.h"
#include "Shield.h"

struct Screen {
    int width;
    int height;
};

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
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

    Screen screen{672, 768}; // original was 224x256    x3 would be 672 x 768
    GameState state = GameState::MAIN_MENU;

    Player player;
    EnemyGrid enemyGrid;
    BulletManager bulletManager;
    std::vector<Shield> shields;

    int score = 0;
    int lives = 3;
    int level = 1;
    float levelCompleteTimer = 0.0f;
    static constexpr float LEVEL_COMPLETE_DELAY = 2.0f;

    // updateScore
    void UpdateScore(int points);
    void InitShields();
    void UpdateLevelComplete(float deltaTime);
    void DrawLevelComplete();
};
