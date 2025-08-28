#include <cstdio>

#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_dma.h>
#include <gba_sprites.h>
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
#include "falcon/gba/draw_text.h"

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
    OBJATTR *oam = OAM;
    // OAM setup (after OBJATTR *oam = OAM;)
    // Player is OBJ 0, obstacles are OBJ 1-4
    oam[0].attr0 = ATTR0_COLOR_16 | ATTR0_SQUARE | (80 & 0xFF); // y
    oam[0].attr1 = ATTR1_SIZE_16 | (120 & 0x1FF); // x
    oam[0].attr2 = 0; // tile 0, palette 0
    int obsX[4] = {50,180,100,160};
    int obsY[4] = {50,60,120,30};
    for (int i = 0; i < 4; ++i) {
        oam[1+i].attr0 = ATTR0_COLOR_16 | ATTR0_SQUARE | (obsY[i] & 0xFF);
        oam[1+i].attr1 = ATTR1_SIZE_16 | (obsX[i] & 0x1FF);
        oam[1+i].attr2 = 32/32; // tile 1, palette 0
    }

    // --- Sprite setup ---
    // Set mode 0, enable OBJ, 1D mapping
    REG_DISPCNT = MODE_0 | OBJ_ENABLE | OBJ_1D_MAP;

    // Simple 16x16 4bpp tile (player: blue, obstacle: white)
    static const u32 playerTile[32] = {0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,
                                       0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,
                                       0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,
                                       0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111};
    static const u32 obstacleTile[32] = {0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,
                                         0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,
                                         0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,
                                         0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222,0x22222222};
    // Palette: 0=transparent, 1=blue, 2=white
    static const u16 spritePalette[16] = {0, RGB5(0,15,31), RGB5(31,31,31)};

    // Copy palette and tiles to VRAM
    DMA3COPY(spritePalette, SPRITE_PALETTE, 16 | DMA16 | DMA_IMMEDIATE);
    DMA3COPY(playerTile, SPRITE_GFX, 32 | DMA32 | DMA_IMMEDIATE);
    DMA3COPY(obstacleTile, SPRITE_GFX+32, 32 | DMA32 | DMA_IMMEDIATE);

    // OAM setup
    // Player is OBJ 0, obstacles are OBJ 1-4
    oam[0].attr0 = ATTR0_COLOR_16 | ATTR0_SQUARE | (80 & 0xFF); // y
    oam[0].attr1 = ATTR1_SIZE_16 | (120 & 0x1FF); // x
    oam[0].attr2 = 0; // tile 0, palette 0
    for (int i = 0; i < 4; ++i) {
        oam[1+i].attr0 = ATTR0_COLOR_16 | ATTR0_SQUARE | (obsY[i] & 0xFF);
        oam[1+i].attr1 = ATTR1_SIZE_16 | (obsX[i] & 0x1FF);
        oam[1+i].attr2 = 32/32; // tile 1, palette 0
    }

    int numObstacles = 4;

    int playerX = 120;
    int playerY = 80;
    int oldPlayerX = playerX;
    int oldPlayerY = playerY;
    KeyInput oldKeys = KeyInput::None;

    while (1) {
        VBlankIntrWait();
        KeyInput keys = pollKeyInput();

        // Move player (simple 4-way, no collision for now)
        if ((keys & KeyInput::Up) != KeyInput::None)    playerY--;
        if ((keys & KeyInput::Down) != KeyInput::None)  playerY++;
        if ((keys & KeyInput::Left) != KeyInput::None)  playerX--;
        if ((keys & KeyInput::Right) != KeyInput::None) playerX++;

        // Clamp to screen
        if (playerX < 0) playerX = 0;
        if (playerX > 240-16) playerX = 240-16;
        if (playerY < 0) playerY = 0;
        if (playerY > 160-16) playerY = 160-16;

        // Update OAM for player
        oam[0].attr0 = (oam[0].attr0 & 0xFF00) | (playerY & 0xFF);
        oam[0].attr1 = (oam[0].attr1 & 0xFE00) | (playerX & 0x1FF);
    }
    return 0;
}