#pragma once
#include "raylib.h"

inline constexpr Color CYAN = {0, 255, 255, 255};

class Utils {
public:

    static void DrawTextCentered(const char* text, int y, int fontSize, Color color);
    static void DrawTextCentered(const char* text, float yPercent, int fontSize, Color color);
    
    // Common positions
    static int TopFifth(int screenHeight)  { return screenHeight / 5; }
    static int TopQuarter(int screenHeight){ return screenHeight / 4; }
    static int TopThird(int screenHeight)  { return screenHeight / 3; }
    static int CenterY(int screenHeight, int fontSize) { return screenHeight/2 - fontSize/2; }
};
