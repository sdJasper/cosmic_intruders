#include "Game.h"

Game::Game() {
    InitWindow(screen.width, screen.height, "Cosmic Intruders");
    SetTargetFPS(60);
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CloseWindow();
}

void Game::Update() {
    switch (state) {
        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::PLAYING;
                enemyGrid.Reset();
                player = Player();           // Reset player
                score = 0;
            }
            break;

        case GameState::PLAYING:
            UpdatePlaying();
            break;

        case GameState::PAUSED:
            if (IsKeyPressed(KEY_P)) {
                state = GameState::PLAYING;
            }
            break;

        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MAIN_MENU;
            }
            break;
    }
}

void Game::UpdatePlaying() {
    float deltaTime = GetFrameTime();
    
    player.Update(deltaTime);
    enemyGrid.Update(deltaTime);

    // TODO: Add collision detection (player bullets vs enemies)
    enemyGrid.CheckHit(player.bullets);
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (state) {
        case GameState::PLAYING:
            DrawPlaying();
            break;

        case GameState::MAIN_MENU:
            DrawText("COSMIC INTRUDERS", 140, 180, 60, RAYWHITE);
            DrawText("Press ENTER to Start", 250, 300, 30, LIGHTGRAY);
            break;

        case GameState::GAME_OVER:
            DrawText("GAME OVER", 250, 200, 60, RED);
            DrawText(TextFormat("Final Score: %i", score), 240, 280, 30, WHITE);
            DrawText("Press ENTER to return to menu", 200, 340, 20, LIGHTGRAY);
            break;

        case GameState::PAUSED:
            DrawText("PAUSED", 320, 240, 50, YELLOW);
            break;
    }

    EndDrawing();
}

void Game::DrawPlaying() {
    player.Draw();
    enemyGrid.Draw();

    // HUD
    DrawText(TextFormat("SCORE: %05i", score), 20, 10, 20, WHITE);
    DrawText(TextFormat("LIVES: %i", lives), 20, 40, 20, WHITE);
    DrawText(TextFormat("LEVEL: %i", level), 650, 10, 20, WHITE);
}
