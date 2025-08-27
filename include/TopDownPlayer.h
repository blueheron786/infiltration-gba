#pragma once
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include "Obstacle.h"

class TopDownPlayer {
public:
	int x, y, w, h;
	u16 color;

	TopDownPlayer(int x, int y, int w, int h, u16 color);
	void draw(u16* fb) const;
	void erase(u16* fb) const;
	void move(u16 keys);
	bool collidesWith(const Obstacle& obs) const;
};
