#pragma once
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include "Obstacle.h"
#include "falcon/gba/display.h"
#include "falcon/gba/key_input.h"
#include "falcon/ecs/entities/Node.h"
#include "falcon/ecs/components/ColourRect.h"
#include "falcon/ecs/components/Collision.h"

class TopDownPlayer : public Node {
public:
    TopDownPlayer(int x, int y, int w, int h, u16 color);
    void move(KeyInput keys);
    bool collidesWith(const Obstacle& obs) const;
    
private:
    void updateCollisionPosition();
};