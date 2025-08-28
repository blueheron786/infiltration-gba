
#include "Obstacle.h"
#include "falcon/gba/display.h"

Obstacle::Obstacle(int x, int y, int w, int h, u16 color)
    : Node(x, y)
{
    addComponent<ColourRect>(w, h, color);
    addComponent<Collision>(CollisionShape::Rectangle, x, y, w, h);
}

void Obstacle::draw(u16* fb) const {
    auto rect = getComponent<ColourRect>();
    if (!rect) return;
    for (int dy = 0; dy < rect->h; ++dy) {
        for (int dx = 0; dx < rect->w; ++dx) {
            drawPixel(fb, x + dx, y + dy, rect->color);
        }
    }
}
