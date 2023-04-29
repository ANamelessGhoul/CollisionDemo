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
    if (positions.size() <= pointIndex || Vector2LengthSqr(displacement) <= (EPSILON))
        return;

    Vector2 newPoint = Vector2Add(positions[pointIndex], displacement);
    if (newPoint.x < -FLT_MAX || newPoint.x > FLT_MAX || newPoint.y < -FLT_MAX || newPoint.y > FLT_MAX) {
        std::cerr << "Invalid point coordinates: (" << newPoint.x << ", " << newPoint.y << ")" << std::endl;
        return;
    }
    quadTree.remove(pointIndex, positions[pointIndex]);
    quadTree.insert(pointIndex, newPoint);
}

