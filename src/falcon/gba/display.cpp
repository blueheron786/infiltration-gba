#include "falcon/gba/display.h"

void drawPixel(uint16_t* fb, int x, int y, uint16_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        fb[y * SCREEN_WIDTH + x] = color;
    }
}
