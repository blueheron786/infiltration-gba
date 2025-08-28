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
    addComponent<PlayerControlled>();
}

bool TopDownPlayer::collidesWith(const Obstacle& obs) const {
    return CollisionSystem::checkCollision(*this, obs);
}