#include <gba.h>
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int w, int h, u16 color)
	: x(x), y(y), w(w), h(h), color(color) {}

void Obstacle::draw(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
				fb[(y + dy) * 240 + (x + dx)] = color;
			}
		}
	}
}
