
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_dma.h>
#include <string.h>
#include <vector>
#include "TopDownPlayer.h"
#include "Obstacle.h"
#include "falcon/ecs/systems/CollisionSystem.h"
#include "falcon/ecs/systems/ColorRectDrawingSystem.h"
#include "falcon/ecs/systems/MovementSystem.h"

// Drawing utilities
#include "falcon/gba/display.h"
#include "falcon/gba/key_input.h"
#include "falcon/gba/falcon.h"

// Simple sound effect (beep)
void playThudSound() {
    // Set up sound channel 1 for a simple square wave
    REG_SOUND1CNT_L = 0x0040; // Sweep off
    REG_SOUND1CNT_H = 0x8000 | (0 << 6); // Square wave, duty cycle 12.5%
    REG_SOUND1CNT_X = 0x8400 | (1000); // Enable, frequency ~1000Hz
}

// Draw button states
void drawButtons(u16* fb, KeyInput keys) {
    u16 grey = RGB5(15, 15, 15);
    u16 blue = RGB5(0, 15, 31);
    
    // A button (right)
    drawRect(fb, 200, 120, 16, 16, (keys & KeyInput::A) != KeyInput::None ? blue : grey);
    
    // B button (left of A)
    drawRect(fb, 180, 130, 16, 16, (keys & KeyInput::B) != KeyInput::None ? blue : grey);
    
    // L button (top left)
    drawRect(fb, 10, 10, 20, 8, (keys & KeyInput::L) != KeyInput::None ? blue : grey);
    
    // R button (top right)
    drawRect(fb, 210, 10, 20, 8, (keys & KeyInput::R) != KeyInput::None ? blue : grey);
}


int main() {
    init();

    // Use VRAM directly but with proper timing
    u16* fb = (u16*)VRAM;

    TopDownPlayer player(120, 80, 16, 16, RGB5(0, 15, 31));
    Obstacle obstacles[] = {
        Obstacle(50, 50, 20, 20, RGB5(31, 31, 31)),
        Obstacle(180, 60, 20, 20, RGB5(31, 31, 31)),
        Obstacle(100, 120, 20, 20, RGB5(31, 31, 31)),
        Obstacle(160, 30, 20, 20, RGB5(31, 31, 31))
    };
    int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);

    int oldPlayerX = player.x;
    int oldPlayerY = player.y;
    KeyInput oldKeys = KeyInput::None;

    // Initial clear and draw
    for (int i = 0; i < 240 * 160; i++) {
        fb[i] = RGB5(0, 0, 0);
    }
    
    // Draw all obstacles using the drawing system
    for (int i = 0; i < numObstacles; i++) {
        ColorRectDrawingSystem::drawEntity(obstacles[i], fb);
    }
    
    // Draw player using the drawing system
    ColorRectDrawingSystem::drawEntity(player, fb);
    drawButtons(fb, KeyInput::None);

    while (1) {
        VBlankIntrWait();
        KeyInput keys = pollKeyInput();
        
        int prevX = player.x;
        int prevY = player.y;

        // Move all player-controlled entities
        std::vector<Entity*> entities = { &player };
        MovementSystem::movePlayerControlledEntities(entities, keys);

        bool collided = false;
        for (int i = 0; i < numObstacles; i++) {
            if (CollisionSystem::checkCollision(player, obstacles[i])) {
                // Reset position on collision
                player.x = oldPlayerX;
                player.y = oldPlayerY;
                // Update collision position after resetting position
                auto collision = player.getComponent<Collision>();
                if (collision) {
                    collision->x = player.x;
                    collision->y = player.y;
                }
                playThudSound();
                collided = true;
                break;
            }
        }

        // Store current position for next frame's collision detection
        if (!collided) {
            oldPlayerX = player.x;
            oldPlayerY = player.y;
        }

        // Only redraw if player actually moved
        if (player.x != prevX || player.y != prevY) {
            // Erase old position using the drawing system
            auto rect = player.getComponent<ColourRect>();
            if (rect) {
                ColorRectDrawingSystem::eraseRect(fb, prevX, prevY, *rect);
            }
            
            // Redraw any obstacles that were at the old position
            for (int i = 0; i < numObstacles; i++) {
                auto obsRect = obstacles[i].getComponent<ColourRect>();
                if (obsRect &&
                    prevX < obstacles[i].x + obsRect->w &&
                    prevX + (rect ? rect->w : 0) > obstacles[i].x &&
                    prevY < obstacles[i].y + obsRect->h &&
                    prevY + (rect ? rect->h : 0) > obstacles[i].y) {
                    ColorRectDrawingSystem::drawEntity(obstacles[i], fb);
                }
            }
            
            // Draw player at new position using the drawing system
            ColorRectDrawingSystem::drawEntity(player, fb);
        }

        if (keys != oldKeys) {
            drawButtons(fb, keys);
            oldKeys = keys;
        }
    }
    return 0;
}