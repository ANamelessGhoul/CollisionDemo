#include "quadTreeWorld.h"
#include "quadTree.h"

QuadTreeWorld::QuadTreeWorld()
{
    quadTree = new QuadTree{this, Rectangle{ -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2 }};
}

QuadTreeWorld::~QuadTreeWorld()
{
    delete quadTree;
}

void QuadTreeWorld::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer)
{
    quadTree->search(bounds, buffer);
}

void QuadTreeWorld::OnPointMoved(size_t pointIndex, Vector2 displacement)
{
    // Remove points that have moved and add them again to update their positions.
    // if (positions.size() <= pointIndex || Vector2LengthSqr(displacement) <= (EPSILON))
    //     return;

    if (Vector2LengthSqr(displacement) < EPSILON)
        return;

    Vector2 newPoint = Vector2Add(positions[pointIndex], displacement);

    quadTree->remove(pointIndex, positions[pointIndex]);
    quadTree->insert(pointIndex, newPoint);
    //positions[pointIndex] = newPoint;
}

