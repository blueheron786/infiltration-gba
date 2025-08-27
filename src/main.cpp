#include <gba.h>
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>


class Obstacle {
public:
    int x, y, w, h;
    u16 color;

    Obstacle(int x, int y, int w, int h, u16 color)
        : x(x), y(y), w(w), h(h), color(color) {}

    void draw(u16* fb) const {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
                    fb[(y + dy) * 240 + (x + dx)] = color;
                }
            }
        }
    }
};

class Player {
public:
    int x, y, w, h;
    u16 color;

    Player(int x, int y, int w, int h, u16 color)
        : x(x), y(y), w(w), h(h), color(color) {}

    void draw(u16* fb) const {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
                    fb[(y + dy) * 240 + (x + dx)] = color;
                }
            }
        }
    }

    void erase(u16* fb) const {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
                    fb[(y + dy) * 240 + (x + dx)] = RGB5(0, 0, 0);
                }
            }
        }
    }

    void move(u16 keys) {
        if (keys & KEY_LEFT) x -= 2;
        if (keys & KEY_RIGHT) x += 2;
        if (keys & KEY_UP) y -= 2;
        if (keys & KEY_DOWN) y += 2;
        if (x < 0) x = 0;
        if (x > 240 - w) x = 240 - w;
        if (y < 0) y = 0;
        if (y > 160 - h) y = 160 - h;
    }

    bool collidesWith(const Obstacle& obs) const {
        return x < obs.x + obs.w &&
               x + w > obs.x &&
               y < obs.y + obs.h &&
               y + h > obs.y;
    }
};

// Function to draw a filled rectangle (used for buttons)
void drawRect(u16* fb, int x, int y, int w, int h, u16 color) {
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            if (x + dx >= 0 && x + dx < 240 && y + dy >= 0 && y + dy < 160) {
                fb[(y + dy) * 240 + (x + dx)] = color;
            }
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
void drawButtons(u16* fb, u16 keys) {
    u16 grey = RGB5(15, 15, 15);
    u16 blue = RGB5(0, 15, 31);
    
    // A button (right)
    drawRect(fb, 200, 120, 16, 16, (keys & KEY_A) ? blue : grey);
    
    // B button (left of A)
    drawRect(fb, 180, 130, 16, 16, (keys & KEY_B) ? blue : grey);
    
    // L button (top left)
    drawRect(fb, 10, 10, 20, 8, (keys & KEY_L) ? blue : grey);
    
    // R button (top right)
    drawRect(fb, 210, 10, 20, 8, (keys & KEY_R) ? blue : grey);
}


int main() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    irqInit();
    irqEnable(IRQ_VBLANK);
    REG_SOUNDCNT_X = 0x0080;
    REG_SOUNDCNT_L = 0x1177;
    REG_SOUNDCNT_H = 0x0002;
    u16* fb = (u16*)VRAM;

    Player player(120, 80, 16, 16, RGB5(0, 15, 31));
    Obstacle obstacles[] = {
        Obstacle(50, 50, 20, 20, RGB5(31, 31, 31)),
        Obstacle(180, 60, 20, 20, RGB5(31, 31, 31)),
        Obstacle(100, 120, 20, 20, RGB5(31, 31, 31)),
        Obstacle(160, 30, 20, 20, RGB5(31, 31, 31))
    };
    int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);

    Player oldPlayer = player;
    u16 oldKeys = 0;

    for (int i = 0; i < 240 * 160; i++) {
        fb[i] = RGB5(0, 0, 0);
    }
    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].draw(fb);
    }
    player.draw(fb);
    drawButtons(fb, 0);

    while (1) {
        VBlankIntrWait();
        scanKeys();
        u16 keys = keysHeld();
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