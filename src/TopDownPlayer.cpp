#include <gba.h>
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include "TopDownPlayer.h"

TopDownPlayer::TopDownPlayer(int x, int y, int w, int h, u16 color)
	: x(x), y(y), w(w), h(h), color(color) {}

void TopDownPlayer::draw(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
				fb[(y + dy) * 240 + (x + dx)] = color;
			}
		}
	}
}

void TopDownPlayer::erase(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
				fb[(y + dy) * 240 + (x + dx)] = RGB5(0, 0, 0);
			}
		}
	}
}

void TopDownPlayer::move(u16 keys) {
	if (keys & KEY_LEFT) x -= 2;
	if (keys & KEY_RIGHT) x += 2;
	if (keys & KEY_UP) y -= 2;
	if (keys & KEY_DOWN) y += 2;
	if (x < 0) x = 0;
	if (x > 240 - w) x = 240 - w;
	if (y < 0) y = 0;
	if (y > 160 - h) y = 160 - h;
}

bool TopDownPlayer::collidesWith(const Obstacle& obs) const {
	return x < obs.x + obs.w &&
		   x + w > obs.x &&
		   y < obs.y + obs.h &&
		   y + h > obs.y;
}
