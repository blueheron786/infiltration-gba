#pragma once

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#include <cstdint>
#include <gba_video.h> // defines u16

void drawPixel(uint16_t* fb, int x, int y, uint16_t color);
void drawRect(u16* fb, int x, int y, int w, int h, u16 color);