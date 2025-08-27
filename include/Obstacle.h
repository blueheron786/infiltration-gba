#pragma once
#include <gba.h>
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

class Obstacle {
public:
	int x, y, w, h;
	u16 color;

	Obstacle(int x, int y, int w, int h, u16 color);
	void draw(u16* fb) const;
};
