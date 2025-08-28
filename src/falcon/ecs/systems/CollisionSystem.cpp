#include "falcon/ecs/systems/CollisionSystem.h"

bool CollisionSystem::checkCollision(const Entity& entity1, const Entity& entity2) {
    auto collision1 = entity1.getComponent<Collision>();
    auto collision2 = entity2.getComponent<Collision>();
    
    if (!collision1 || !collision2) {
        return false;
    }
    
    // Currently only supporting rectangle collision
    if (collision1->shape != CollisionShape::Rectangle || 
        collision2->shape != CollisionShape::Rectangle) {
        return false;
    }
    
    // AABB (Axis-Aligned Bounding Box) collision detection
    return collision1->x < collision2->x + collision2->width &&
           collision1->x + collision1->width > collision2->x &&
           collision1->y < collision2->y + collision2->height &&
           collision1->y + collision1->height > collision2->y;
}
