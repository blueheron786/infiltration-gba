#pragma once
#include <gba_video.h>
#include <gba_input.h>
#include <gba_sound.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

/**
 * Falcon GBA Library - Main initialization and utility functions
 */

/**
 * Initialize the GBA system for basic operation
 * Sets up video mode, interrupts, and sound
 */
void init();
