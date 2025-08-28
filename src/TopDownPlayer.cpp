#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

#include "TopDownPlayer.h"
#include "falcon/gba/display.h"
#include "falcon/gba/key_input.h"

TopDownPlayer::TopDownPlayer(int x, int y, int w, int h, u16 color)
	: Node(x, y), w(w), h(h), color(color) {}

void TopDownPlayer::draw(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			drawPixel(fb, x + dx, y + dy, color);
		}
	}
}

void TopDownPlayer::erase(u16* fb) const {
	for (int dy = 0; dy < h; ++dy) {
		for (int dx = 0; dx < w; ++dx) {
			drawPixel(fb, x + dx, y + dy, RGB5(0, 0, 0));
		}
	}
}

void TopDownPlayer::move(KeyInput keys) {
	if ((keys & KeyInput::Left) != KeyInput::None) x -= 2;
	if ((keys & KeyInput::Right) != KeyInput::None) x += 2;
	if ((keys & KeyInput::Up) != KeyInput::None) y -= 2;
	if ((keys & KeyInput::Down) != KeyInput::None) y += 2;
	
	if (x < 0) x = 0;
	if (x > SCREEN_WIDTH - w) x = SCREEN_WIDTH - w;
	if (y < 0) y = 0;
	if (y > SCREEN_HEIGHT - h) y = SCREEN_HEIGHT - h;
}

bool TopDownPlayer::collidesWith(const Obstacle& obs) const {
	return x < obs.x + obs.w &&
		   x + w > obs.x &&
		   y < obs.y + obs.h &&
		   y + h > obs.y;
}
