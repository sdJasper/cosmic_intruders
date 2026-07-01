#include "Shield.h"
#include <cmath>

Shield::Shield() {
    Reset();
}


void Shield::Reset() {
    cells.assign(COLS * ROWS, true);
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            bool alive = true;

            // === TOP ROUNDED ARCH / CORNERS ===
            if (row < 4) {
                if (col < 3 && row < (3 - col)) alive = false;
                if (col > COLS - 4 && row < (3 - (COLS - 1 - col))) alive = false;
            }
            // === BOTTOM DOORWAY (smooth U-shape) ===
            if (row >= ROWS - 5) {
                int distFromCenter = std::abs(col - COLS/2);
                if (distFromCenter <= 3) {
                    // Smooth U cutout
                    if (row >= ROWS - 3 || (row == ROWS - 4 && distFromCenter <= 2)) {
                        alive = false;
                    }
                }
            }

            cells[GetIndex(col, row)] = alive;
        }
    }
}


Rectangle Shield::GetBounds() const {
    return { position.x, position.y, (float)(COLS * CELL_SIZE), (float)(ROWS * CELL_SIZE) };
}

bool Shield::CheckHit(Vector2 point, float radius) {
    Rectangle bounds = GetBounds();
    if (!CheckCollisionCircleRec(point, radius, bounds)) return false;

    int col = (int)((point.x - position.x) / CELL_SIZE);
    int row = (int)((point.y - position.y) / CELL_SIZE);

    if (col < 0 || col >= COLS || row < 0 || row >= ROWS) return false;
    if (!cells[GetIndex(col, row)]) return false; // already destroyed here

    // Carve out a small blast radius for a chunkier look
    int blast = 2;
    for (int dy = -blast; dy <= blast; dy++) {
        for (int dx = -blast; dx <= blast; dx++) {
            int c = col + dx, r = row + dy;
            if (c >= 0 && c < COLS && r >= 0 && r < ROWS) {
                if (std::abs(dx) + std::abs(dy) <= blast) // diamond-shaped blast
                    cells[GetIndex(c, r)] = false;
            }
        }
    }
    return true;
}

void Shield::Draw() const {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (cells[GetIndex(col, row)]) {
                DrawRectangle(
                    position.x + col * CELL_SIZE,
                    position.y + row * CELL_SIZE,
                    CELL_SIZE, CELL_SIZE,
                    GREEN
                );
            }
        }
    }
}

void Shield::EraseOverlap(Rectangle enemyRect) {
    Rectangle bounds = GetBounds();
    if (!CheckCollisionRecs(bounds, enemyRect)) return; // quick reject

    // find which cells the enemy rect covers and destroy them
    int colStart = (int)((enemyRect.x - position.x) / CELL_SIZE);
    int colEnd   = (int)((enemyRect.x + enemyRect.width - position.x) / CELL_SIZE);
    int rowStart = (int)((enemyRect.y - position.y) / CELL_SIZE);
    int rowEnd   = (int)((enemyRect.y + enemyRect.height - position.y) / CELL_SIZE);

    // clamp to shield bounds
    colStart = std::max(colStart, 0);
    colEnd   = std::min(colEnd,   COLS - 1);
    rowStart = std::max(rowStart, 0);
    rowEnd   = std::min(rowEnd,   ROWS - 1);

    for (int row = rowStart; row <= rowEnd; row++) {
        for (int col = colStart; col <= colEnd; col++) {
            cells[GetIndex(col, row)] = false;
        }
    }
}
