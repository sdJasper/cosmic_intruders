#include "Game.h"
#include "Utils.h"
#include "Player.h"
#include "Shield.h"
#include <string>

Game::Game() {
    InitWindow(screen.width, screen.height, "Cosmic Intruders");
    SetTargetFPS(60);
    bulletManager = BulletManager();
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
                enemyGrid.Reset();
                player = Player();
                InitShields();
                score = 0;
                lives = 3;
                level = 1;
                state = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:
            if (IsKeyPressed(KEY_P)) {
                state = GameState::PAUSED;
                break;
            }
            UpdatePlaying();
            break;

        case GameState::PAUSED:
            if (IsKeyPressed(KEY_P)) {
                state = GameState::PLAYING;
            }
            break;
        
        case GameState::LEVEL_COMPLETE:
            bulletManager.Reset();
            UpdateLevelComplete(GetFrameTime());
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
    
    player.Update(deltaTime, bulletManager);
    enemyGrid.Update(deltaTime, bulletManager);
    bulletManager.Update(deltaTime);

    bool playerHit = false;
    score += bulletManager.CheckCollisions(enemyGrid, player, shields, playerHit);

    if (playerHit) {
        lives -= 1;
        if (lives <= 0) state = GameState::GAME_OVER;
    }

    if (enemyGrid.GetAliveCount() < 1) {
        state = GameState::LEVEL_COMPLETE;
        levelCompleteTimer = 0.0f;
    }
}

void Game::UpdateLevelComplete(float deltaTime) {
    levelCompleteTimer += deltaTime;
    if (levelCompleteTimer >= LEVEL_COMPLETE_DELAY) {
        level++;
        enemyGrid.Reset(level);   // pass level so grid can start lower
        bulletManager.Reset(); // deactivate all bullets mid-flight
        // shields intentionally NOT reset
        state = GameState::PLAYING;
    }
}

void Game::DrawLevelComplete() {
    Utils::DrawTextCentered(TextFormat("WAVE %d CLEAR", level), GetScreenWidth()/2 - 100, 30, GREEN);
    Utils::DrawTextCentered(TextFormat("SCORE: %d", score), GetScreenHeight()/2 + 40, 20, WHITE);
}


void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (state) {
        case GameState::PLAYING:
            DrawPlaying();
            for (auto& shield : shields) {
               shield.Draw();
            }
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
            DrawPlaying();
            for (auto& shield : shields) {
               shield.Draw();
            }
            Utils::DrawTextCentered("PAUSED", 0.5f, 50, YELLOW);
            Utils::DrawTextCentered("Press P to Resume", 0.6f, 30, LIGHTGRAY);
            break;

        case GameState::LEVEL_COMPLETE:
            DrawPlaying();
            DrawLevelComplete();
            break;

    }

    EndDrawing();
}

void Game::DrawPlaying() {
    player.Draw();
    enemyGrid.Draw();
    bulletManager.Draw();

    // HUD
    // Utils::DrawTextCentered(TextFormat("SCORE: %05i", score), 0.05f, 20, WHITE);
    Utils::DrawTextCentered("SCORE< 1 >    HI-SCORE    SCORE< 2 >", 0.05f, 30, WHITE);
    Utils::DrawTextCentered(TextFormat("%04i         %04i         %04i", score, 0, 0), 0.10f, 35, WHITE);

    // DrawLine(0, GetScreenHeight() * 0.93f, GetScreenWidth(), GetScreenHeight() * 0.93f, RED);
    DrawLineEx(Vector2{0, GetScreenHeight() * 0.93f}, Vector2{GetScreenWidth()*1.0f, GetScreenHeight() * 0.93f}, 3.0, RED);
    DrawText(TextFormat("%i", lives), GetScreenWidth() * 0.05f, GetScreenHeight() * 0.935f, 30, CYAN);
    int offset = GetScreenWidth() * 0.12f;
    for (int i = 1; i < lives; i++) {
        Player::DrawShip(offset, GetScreenHeight() * 0.95f);
        offset += PLAYER_WIDTH + 10;
    }
}

void Game::UpdateScore(int points) {
    score += points;
}

void Game::InitShields() {
    shields.clear();

    const int shieldCount = 4;
    const float shieldWidth = Shield::COLS * Shield::CELL_SIZE;
    const float screenWidth = (float)GetScreenWidth();
    const float marginY = 100.0f + (Shield::COLS * Shield::CELL_SIZE); // distance above player

    // evenly distribute shields across the width, with margins on the outer edges
    float totalGapSpace = screenWidth - (shieldCount * shieldWidth);
    float gap = totalGapSpace / (shieldCount + 1);

    for (int i = 0; i < shieldCount; i++) {
        Shield shield;
        shield.position = {
            gap + i * (shieldWidth + gap),
            (float)GetScreenHeight() - marginY - (Shield::ROWS * Shield::CELL_SIZE)
        };
        shield.Reset(); // carves the default silhouette, all cells intact
        shields.push_back(shield);
    }
}
