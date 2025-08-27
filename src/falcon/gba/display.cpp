#include "falcon/gba/display.h"
#include <gba_video.h>

void drawPixel(uint16_t* fb, int x, int y, uint16_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        fb[y * SCREEN_WIDTH + x] = color;
    }
}

void drawRect(u16* fb, int x, int y, int w, int h, u16 color) {
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            drawPixel(fb, x + dx, y + dy, color);
        }
    }
}