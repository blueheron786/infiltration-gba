#pragma once
#include "../entities/Entity.h"
#include "../components/ColourRect.h"
#include <gba_video.h>
#include <vector>

class ColorRectDrawingSystem {
public:
    // Draw all entities with ColourRect components
    static void drawAll(const std::vector<Entity*>& entities, u16* framebuffer);
    
    // Draw a specific entity with ColourRect component
    static void drawEntity(const Entity& entity, u16* framebuffer);
    
    // Erase (draw black) a specific entity with ColourRect component
    static void eraseEntity(const Entity& entity, u16* framebuffer);
    
    // Draw a colored rectangle at specific position (useful for erasing old positions)
    static void drawRect(u16* framebuffer, int x, int y, const ColourRect& rect);
    
    // Erase (draw black) a colored rectangle at specific position
    static void eraseRect(u16* framebuffer, int x, int y, const ColourRect& rect);
};
