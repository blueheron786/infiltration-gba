#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

#include "TopDownPlayer.h"
#include "falcon/gba/display.h"
#include "falcon/gba/key_input.h"
#include "falcon/ecs/systems/CollisionSystem.h"

TopDownPlayer::TopDownPlayer(int x, int y, int w, int h, u16 color)
    : Node(x, y)
{
    addComponent<ColourRect>(w, h, color);
    addComponent<Collision>(CollisionShape::Rectangle, x, y, w, h);
}void TopDownPlayer::draw(u16* fb) const {
	auto rect = getComponent<ColourRect>();
	if (!rect) return;
	for (int dy = 0; dy < rect->h; ++dy) {
		for (int dx = 0; dx < rect->w; ++dx) {
			drawPixel(fb, x + dx, y + dy, rect->color);
		}
	}
}

void TopDownPlayer::erase(u16* fb) const {
	auto rect = getComponent<ColourRect>();
	if (!rect) return;
	for (int dy = 0; dy < rect->h; ++dy) {
		for (int dx = 0; dx < rect->w; ++dx) {
			drawPixel(fb, x + dx, y + dy, RGB5(0, 0, 0));
		}
	}
}

void TopDownPlayer::move(KeyInput keys) {
    auto rect = getComponent<ColourRect>();
    if (!rect) return;
    if ((keys & KeyInput::Left) != KeyInput::None) x -= 2;
    if ((keys & KeyInput::Right) != KeyInput::None) x += 2;
    if ((keys & KeyInput::Up) != KeyInput::None) y -= 2;
    if ((keys & KeyInput::Down) != KeyInput::None) y += 2;

    if (x < 0) x = 0;
    if (x > SCREEN_WIDTH - rect->w) x = SCREEN_WIDTH - rect->w;
    if (y < 0) y = 0;
    if (y > SCREEN_HEIGHT - rect->h) y = SCREEN_HEIGHT - rect->h;
    
    updateCollisionPosition();
}

void TopDownPlayer::updateCollisionPosition() {
    auto collision = getComponent<Collision>();
    if (collision) {
        collision->x = x;
        collision->y = y;
    }
}

bool TopDownPlayer::collidesWith(const Obstacle& obs) const {
    return CollisionSystem::checkCollision(*this, obs);
}