
#include "TopDownPlayer.h"
#include "Obstacle.h"

// Drawing utilities
#include "falcon/gba/display.h"
#include "falcon/gba/key_input.h"

void drawRect(u16* fb, int x, int y, int w, int h, u16 color) {
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            drawPixel(fb, x + dx, y + dy, color);
        }
    }
}

// ...existing code...

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
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_SOUNDCNT_X = 0x0080;
    REG_SOUNDCNT_L = 0x1177;
    REG_SOUNDCNT_H = 0x0002;
    u16* fb = (u16*)VRAM;

    TopDownPlayer player(120, 80, 16, 16, RGB5(0, 15, 31));
    Obstacle obstacles[] = {
        Obstacle(50, 50, 20, 20, RGB5(31, 31, 31)),
        Obstacle(180, 60, 20, 20, RGB5(31, 31, 31)),
        Obstacle(100, 120, 20, 20, RGB5(31, 31, 31)),
        Obstacle(160, 30, 20, 20, RGB5(31, 31, 31))
    };
    int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);

    TopDownPlayer oldPlayer = player;
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
    oldPlayer = player;

    player.move(keys);

        bool collided = false;
        for (int i = 0; i < numObstacles; i++) {
            if (player.collidesWith(obstacles[i])) {
                player = oldPlayer;
                playThudSound();
                collided = true;
                break;
            }
        }

        if (player.x != oldPlayer.x || player.y != oldPlayer.y) {
            oldPlayer.erase(fb);
            for (int i = 0; i < numObstacles; i++) {
                if (oldPlayer.collidesWith(obstacles[i])) {
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
        oldPlayer = player;
    }
    return 0;
}