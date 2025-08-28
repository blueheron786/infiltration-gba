#pragma once
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>

#include <gba_interrupt.h>
#include "falcon/ecs/entities/Node.h"
#include "falcon/ecs/components/ColourRect.h"
#include "falcon/ecs/components/Collision.h"

class Obstacle : public Node {
public:
    Obstacle(int x, int y, int w, int h, u16 color);
};
