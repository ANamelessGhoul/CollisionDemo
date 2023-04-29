#ifndef QUAD_TREE_WORLD_H
#define QUAD_TREE_WORLD_H

#include "world.h"
#include "quadTree.h"
#include <iostream>

class QuadTreeWorld : public World
{
private:
    QuadTree quadTree;
public:
    QuadTreeWorld();
    ~QuadTreeWorld();
    void CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer) override;
    void OnPointMoved(size_t pointIndex, Vector2 displacement) override;
};

#endif // QUAD_TREE_WORLD_H