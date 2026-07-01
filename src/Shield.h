#pragma once
#include "raylib.h"
#include <vector>

class Shield {
private:
    int GetIndex(int col, int row) const { return row * COLS + col; }

public:
    static constexpr int COLS = 22;
    static constexpr int ROWS = 16;
    static constexpr int CELL_SIZE = 4; // pixels per block

    Vector2 position; // top-left
    std::vector<bool> cells; // true = intact

    Shield();
    void Reset();
    Rectangle GetBounds() const;
    bool CheckHit(Vector2 point, float radius);
    void Draw() const;
    void EraseOverlap(Rectangle enemyRect);
};
