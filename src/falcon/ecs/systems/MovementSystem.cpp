#include "falcon/ecs/systems/MovementSystem.h"
#include "falcon/ecs/entities/Node.h"
#include "falcon/gba/display.h"

void MovementSystem::movePlayerControlledEntities(const std::vector<Entity*>& entities, KeyInput keys) {
    for (auto& entity : entities) {
        if (entity && entity->getComponent<PlayerControlled>()) {
            moveEntity(*entity, keys);
        }
    }
}

void MovementSystem::moveEntity(Entity& entity, KeyInput keys) {
    // Only move entities that have ColourRect and PlayerControlled components
    auto rect = entity.getComponent<ColourRect>();
    auto playerControlled = entity.getComponent<PlayerControlled>();
    if (!rect || !playerControlled) return;
    
    // Try to get position from Node (if entity is a Node)
    Node* node = dynamic_cast<Node*>(&entity);
    if (!node) return;
    
    // Apply movement based on input
    if ((keys & KeyInput::Left) != KeyInput::None) node->x -= 2;
    if ((keys & KeyInput::Right) != KeyInput::None) node->x += 2;
    if ((keys & KeyInput::Up) != KeyInput::None) node->y -= 2;
    if ((keys & KeyInput::Down) != KeyInput::None) node->y += 2;

    // Clamp to screen boundaries
    if (node->x < 0) node->x = 0;
    if (node->x > SCREEN_WIDTH - rect->w) node->x = SCREEN_WIDTH - rect->w;
    if (node->y < 0) node->y = 0;
    if (node->y > SCREEN_HEIGHT - rect->h) node->y = SCREEN_HEIGHT - rect->h;
    
    // Update collision position to match new position
    updateCollisionPosition(entity);
}

void MovementSystem::updateCollisionPosition(Entity& entity) {
    auto collision = entity.getComponent<Collision>();
    Node* node = dynamic_cast<Node*>(&entity);
    
    if (collision && node) {
        collision->x = node->x;
        collision->y = node->y;
    }
}
