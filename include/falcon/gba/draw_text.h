#pragma once
#include <gba_video.h>

// Draw a single digit at (x, y) in color, 5x7 font
void drawDigit(u16* fb, int x, int y, int digit, u16 color);
// Draw an integer at (x, y)
void drawInt(u16* fb, int x, int y, int value, u16 color);
