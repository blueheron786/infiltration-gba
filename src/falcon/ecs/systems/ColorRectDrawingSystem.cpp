#include "falcon/ecs/systems/ColorRectDrawingSystem.h"
#include "falcon/ecs/entities/Node.h"
#include "falcon/gba/display.h"

void ColorRectDrawingSystem::drawAll(const std::vector<Entity*>& entities, u16* framebuffer) {
    for (const auto& entity : entities) {
        if (entity) {
            drawEntity(*entity, framebuffer);
        }
    }
}

void ColorRectDrawingSystem::drawEntity(const Entity& entity, u16* framebuffer) {
    auto rect = entity.getComponent<ColourRect>();
    if (!rect) return;
    
    // Try to get position from Node (if entity is a Node)
    const Node* node = dynamic_cast<const Node*>(&entity);
    if (!node) return;
    
    for (int dy = 0; dy < rect->h; ++dy) {
        for (int dx = 0; dx < rect->w; ++dx) {
            drawPixel(framebuffer, node->x + dx, node->y + dy, rect->color);
        }
    }
}

void ColorRectDrawingSystem::eraseEntity(const Entity& entity, u16* framebuffer) {
    auto rect = entity.getComponent<ColourRect>();
    if (!rect) return;
    
    // Try to get position from Node (if entity is a Node)
    const Node* node = dynamic_cast<const Node*>(&entity);
    if (!node) return;
    
    for (int dy = 0; dy < rect->h; ++dy) {
        for (int dx = 0; dx < rect->w; ++dx) {
            drawPixel(framebuffer, node->x + dx, node->y + dy, RGB5(0, 0, 0));
        }
    }
}

void ColorRectDrawingSystem::drawRect(u16* framebuffer, int x, int y, const ColourRect& rect) {
    for (int dy = 0; dy < rect.h; ++dy) {
        for (int dx = 0; dx < rect.w; ++dx) {
            drawPixel(framebuffer, x + dx, y + dy, rect.color);
        }
    }
}

void ColorRectDrawingSystem::eraseRect(u16* framebuffer, int x, int y, const ColourRect& rect) {
    for (int dy = 0; dy < rect.h; ++dy) {
        for (int dx = 0; dx < rect.w; ++dx) {
            drawPixel(framebuffer, x + dx, y + dy, RGB5(0, 0, 0));
        }
    }
}
