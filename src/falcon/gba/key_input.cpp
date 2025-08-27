#include <gba_input.h>
#include "falcon/gba/key_input.h"

KeyInput pollKeyInput() {
    u16 keys = ~REG_KEYINPUT & 0x03FF;
    KeyInput result = KeyInput::None;
    if (keys & KEY_LEFT) result = result | KeyInput::Left;
    if (keys & KEY_RIGHT) result = result | KeyInput::Right;
    if (keys & KEY_UP) result = result | KeyInput::Up;
    if (keys & KEY_DOWN) result = result | KeyInput::Down;
    if (keys & KEY_A) result = result | KeyInput::A;
    if (keys & KEY_B) result = result | KeyInput::B;
    if (keys & KEY_L) result = result | KeyInput::L;
    if (keys & KEY_R) result = result | KeyInput::R;
    return result;
}
