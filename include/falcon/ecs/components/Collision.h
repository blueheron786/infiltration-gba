#pragma once
#include "Component.h"

enum class CollisionShape {
    Rectangle
};

class Collision : public Component {
public:
    CollisionShape shape;
    int x, y, width, height;

    Collision(CollisionShape shape, int x, int y, int width, int height)
        : shape(shape), x(x), y(y), width(width), height(height) {}
};
