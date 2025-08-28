
#include "TopDownPlayer.h"
#include "Obstacle.h"

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

    // Initialize framebuffer
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

    for (int i = 0; i < 240 * 160; i++) {
        fb[i] = RGB5(0, 0, 0);
    }
    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].draw(fb);
    }
    player.draw(fb);
    drawButtons(fb, KeyInput::None);

    while (1) {
        VBlankIntrWait();
        KeyInput keys = pollKeyInput();
        oldPlayerX = player.x;
        oldPlayerY = player.y;

        player.move(keys);

        bool collided = false;
        for (int i = 0; i < numObstacles; i++) {
            if (player.collidesWith(obstacles[i])) {
                player.x = oldPlayerX;
                player.y = oldPlayerY;
                playThudSound();
                collided = true;
                break;
            }
        }

        if (player.x != oldPlayerX || player.y != oldPlayerY) {
            // Erase old position by drawing a black rectangle
            auto rect = player.getComponent<ColourRect>();
            if (rect) {
                for (int dy = 0; dy < rect->h; ++dy) {
                    for (int dx = 0; dx < rect->w; ++dx) {
                        drawPixel(fb, oldPlayerX + dx, oldPlayerY + dy, RGB5(0, 0, 0));
                    }
                }
            }
            for (int i = 0; i < numObstacles; i++) {
                // Check if old position overlapped with obstacles and redraw them
                auto obsRect = obstacles[i].getComponent<ColourRect>();
                if (obsRect &&
                    oldPlayerX < obstacles[i].x + obsRect->w &&
                    oldPlayerX + (rect ? rect->w : 0) > obstacles[i].x &&
                    oldPlayerY < obstacles[i].y + obsRect->h &&
                    oldPlayerY + (rect ? rect->h : 0) > obstacles[i].y) {
                    obstacles[i].draw(fb);
                }
            }
            player.draw(fb);
        }

        if (keys != oldKeys) {
            drawRect(fb, 200, 120, 16, 16, RGB5(0, 0, 0));
            drawRect(fb, 180, 130, 16, 16, RGB5(0, 0, 0));
            drawRect(fb, 10, 10, 20, 8, RGB5(0, 0, 0));
            drawRect(fb, 210, 10, 20, 8, RGB5(0, 0, 0));
            drawButtons(fb, keys);
            oldKeys = keys;
        }
        oldPlayerX = player.x;
        oldPlayerY = player.y;
    }
    return 0;
}