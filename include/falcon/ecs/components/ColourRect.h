#pragma once
#include "Component.h"
#include <gba_types.h>

class ColourRect : public Component {
public:
    int w, h;
    u16 color;

    ColourRect(int w, int h, u16 color)
        : w(w), h(h), color(color) {}
};
