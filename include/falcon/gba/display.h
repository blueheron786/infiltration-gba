#pragma once

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#include <cstdint>

void drawPixel(uint16_t* fb, int x, int y, uint16_t color);
