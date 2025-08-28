#pragma once
#include "../entities/Entity.h"
#include "../components/ColourRect.h"
#include "../components/Collision.h"
#include "../components/PlayerControlled.h"
#include "../../gba/key_input.h"
#include <vector>

class MovementSystem {
public:
    // Move all player-controlled entities based on input
    static void movePlayerControlledEntities(const std::vector<Entity*>& entities, KeyInput keys);
    
    // Move a specific entity based on input (if it's player-controlled)
    static void moveEntity(Entity& entity, KeyInput keys);
    
    // Update collision position to match entity position
    static void updateCollisionPosition(Entity& entity);
};
