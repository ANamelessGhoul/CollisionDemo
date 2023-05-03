#include "uniformGridWorld.h"


UniformGridWorld::UniformGridWorld() : uniformGrid(200)
{
    for (size_t i = 0; i < DESIRED_COUNT; i++)
    {
        OnPointAdded(i);
    }
}

UniformGridWorld::~UniformGridWorld()
{
}

void Draw2DGrid(int columns, int rows, int step, Color gridColor)
{
    const int maxWidth = columns * step;
    const int maxHeight = rows * step;
    int x = 0;
    for (int i = 0; i < columns; i++)
    {
        DrawLine(x, 0, x, maxHeight, gridColor);
        x += step;
    }

    int y = 0;
    for (int i = 0; i < rows; i++)
    {
        DrawLine(0, y, maxWidth, y, gridColor);
        y += step;
    }
}

void UniformGridWorld::Draw()
{
    World::Draw();
    Draw2DGrid(4, 4, 200, GRAY);
}


void UniformGridWorld::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer)
{
    uniformGrid.Query(bounds, buffer);
}


void UniformGridWorld::OnPointAdded(size_t pointIndex)
{
    uniformGrid.Insert(pointIndex, GetCircleBounds(positions[pointIndex], radiuses[pointIndex]));
}

void UniformGridWorld::OnPointMoved(size_t pointIndex, Vector2 displacement)
{
    // Remove points that have moved and add them again to update their positions.
    if (Vector2LengthSqr(displacement) < 0.01f)
        return;
    Rectangle bounds = GetCircleBounds(positions[pointIndex], radiuses[pointIndex]);
    uniformGrid.Remove(pointIndex, bounds);
    bounds.x += displacement.x;
    bounds.y += displacement.y;
    uniformGrid.Insert(pointIndex, bounds);
}
