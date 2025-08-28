
#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int w, int h, u16 color)
    : Node(x, y)
{
    addComponent<ColourRect>(w, h, color);
    addComponent<Collision>(CollisionShape::Rectangle, x, y, w, h);
}
