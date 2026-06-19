#include "Game.h"
#include "../Utils/Utils.h"

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
    float deltaTime = GetFrameTime();

    switch (state) {
        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::PLAYING;
                // TODO: Create player and enemies later
            }
            break;

        case GameState::PLAYING:
            world.Update(deltaTime);
            // TODO: Add input, collision, scoring, etc.
            if (IsKeyPressed(KEY_P)) {
                state = GameState::PAUSED;
            }
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
            score = 0;
            lives = 3;
            level = 1;
            break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (state) {
        case GameState::PLAYING:
        world.Draw();
        break;
        
        case GameState::MAIN_MENU: {
            Utils::DrawTextCentered("COSMIC INTRUDERS", 0.33f, 60, RAYWHITE);
            Utils::DrawTextCentered("Press ENTER to Start", 0.5f, 30, LIGHTGRAY);
            break;
        }
        case GameState::GAME_OVER:
            Utils::DrawTextCentered("GAME OVER", 0.33f, 60, RED);
            Utils::DrawTextCentered(TextFormat("Final Score: %i", score), 0.5f, 30, WHITE);
            Utils::DrawTextCentered("Press ENTER to return to menu", 0.6f, 20, LIGHTGRAY);
            break;

        case GameState::PAUSED:
            world.Draw();
            Utils::DrawTextCentered("PAUSED", 0.5f, 50, YELLOW);
            Utils::DrawTextCentered("Press P to Resume", 0.6f, 30, LIGHTGRAY);
            break;
    }

    EndDrawing();
}

// void Game::DrawPlaying() {
//     player.Draw();
//     enemyGrid.Draw();

//     // HUD
//     // Utils::DrawTextCentered(TextFormat("SCORE: %05i", score), 0.05f, 20, WHITE);
//     Utils::DrawTextCentered("SCORE< 1 >    HI-SCORE    SCORE< 2 >", 0.05f, 30, WHITE);
//     Utils::DrawTextCentered(TextFormat("%04i         %04i         %04i", score, 0, 0), 0.10f, 35, WHITE);

//     // DrawLine(0, GetScreenHeight() * 0.93f, GetScreenWidth(), GetScreenHeight() * 0.93f, RED);
//     DrawLineEx(Vector2{0, GetScreenHeight() * 0.93f}, Vector2{GetScreenWidth()*1.0f, GetScreenHeight() * 0.93f}, 3.0, RED);
//     DrawText(TextFormat("%i", lives), GetScreenWidth() * 0.05f, GetScreenHeight() * 0.935f, 30, CYAN);
//     int offset = GetScreenWidth() * 0.12f;
//     for (int i = 1; i < lives; i++) {
//         Player::DrawShip(offset, GetScreenHeight() * 0.95f);
//         offset += PLAYER_WIDTH + 10;
//     }
//     // Utils::DrawTextCentered(TextFormat("LIVES: %i", lives), 0.1f, 20, WHITE);
//     // Utils::DrawTextCentered(TextFormat("LEVEL: %i", level), 0.95f, 20, WHITE);
// }
