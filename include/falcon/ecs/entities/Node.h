#pragma once

class Node {
public:
    int x, y;
    
    Node(int x, int y);
    virtual ~Node() = default;
};
