#pragma once
#include "../entities/Entity.h"
#include "../components/Collision.h"

class CollisionSystem {
public:
    static bool checkCollision(const Entity& entity1, const Entity& entity2);
};
