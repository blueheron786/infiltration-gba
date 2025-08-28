#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

#include "falcon/gba/falcon.h"

void init() {
    // Set video mode to Mode 3 (bitmap mode) with background 2 enabled
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    
    // Initialize interrupts
    irqInit();
    irqEnable(IRQ_VBLANK);
    
    // Initialize sound system
    REG_SOUNDCNT_X = 0x0080;  // Enable sound
    REG_SOUNDCNT_L = 0x1177;  // Set volume and enable channels
    REG_SOUNDCNT_H = 0x0002;  // Set sound control
}
