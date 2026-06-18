#include "Utils.h"

void Utils::DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int centerX = GetScreenWidth() / 2 - textWidth / 2;
    DrawText(text, centerX, y, fontSize, color);
}

void Utils::DrawTextCentered(const char* text, float yPercent, int fontSize, Color color) {
    int y = (int)(GetScreenHeight() * yPercent);
    DrawTextCentered(text, y, fontSize, color);
}
