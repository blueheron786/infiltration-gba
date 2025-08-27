#pragma once

#include <cstdint>

// Enum for key input
enum class KeyInput {
    None = 0,
    Left = 1 << 0,
    Right = 1 << 1,
    Up = 1 << 2,
    Down = 1 << 3,
    A = 1 << 4,
    B = 1 << 5,
    L = 1 << 6,
    R = 1 << 7
};

// Bitmask operators for KeyInput
enum class KeyInput;
inline KeyInput operator|(KeyInput a, KeyInput b) { return static_cast<KeyInput>(static_cast<int>(a) | static_cast<int>(b)); }
inline KeyInput operator&(KeyInput a, KeyInput b) { return static_cast<KeyInput>(static_cast<int>(a) & static_cast<int>(b)); }

KeyInput pollKeyInput();
