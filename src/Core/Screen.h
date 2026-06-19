#pragma once

struct Screen {
    int width;
    int height;

    Screen(int w = 800, int h = 600)
        : width(w), height(h) {}
};
