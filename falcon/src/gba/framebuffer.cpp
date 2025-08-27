#include <falcon/gba/gba.h>
#include <falcon/gba/framebuffer.h>

namespace falcon {
namespace gba {

    void writePixel(int x, int y, unsigned short color) {
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
            unsigned short* fb = (unsigned short*)0x06000000;
            fb[y * SCREEN_WIDTH + x] = color;
        }
    }

    void drawRect(unsigned short* fb, int x, int y, int w, int h, unsigned short color) {
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (x + dx >= 0 && x + dx < SCREEN_WIDTH && y + dy >= 0 && y + dy < SCREEN_HEIGHT) {
                    fb[(y + dy) * SCREEN_WIDTH + (x + dx)] = color;
                }
            }
        }
    }

    void fillScreen(unsigned short* fb, unsigned short color) {
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
            fb[i] = color;
        }
    }

}
}
