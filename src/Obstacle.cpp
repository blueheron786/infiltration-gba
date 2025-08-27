
#include "Obstacle.h"
#include "falcon/gba/display.h"

Obstacle::Obstacle(int x, int y, int w, int h, u16 color)
	: x(x), y(y), w(w), h(h), color(color) {}

void Obstacle::draw(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			drawPixel(fb, x + dx, y + dy, color);
		}
	}
}
