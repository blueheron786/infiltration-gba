
#pragma once
#include "Entity.h"

class Node : public Entity {
public:
    int x, y;

    Node(int x, int y);
    virtual ~Node() = default;
};
