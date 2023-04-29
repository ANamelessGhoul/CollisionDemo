#include "quadTreeWorld.h"


QuadTreeWorld::QuadTreeWorld() : quadTree(Rectangle{ -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2 })
{
}

QuadTreeWorld::~QuadTreeWorld()
{
}

void QuadTreeWorld::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer)
{
    quadTree.search(bounds, buffer);
}

void QuadTreeWorld::OnPointMoved(size_t pointIndex, Vector2 displacement)
{
    // Remove points that have moved and add them again to update their positions.
}
