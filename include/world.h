#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "random.h"
#include "raylib.h"
#include "raymath.h"
static const unsigned long long DESIRED_COUNT = {1000ull};

class World
{
private:
    void CreateRandomPoint();
    
protected:
    int screenWidth, screenHeight;
    Rectangle screenRect;
    unsigned long long objectCount;
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<float> radiuses;

    std::vector<size_t> screenCollisionsBuffer;
    std::vector<size_t> collisionsBuffer;

    const float WORLD_SIZE = { 500.0f };

public:
    int PointsSize();
    Vector2& GetPosition(int index);
    virtual void DrawPoints();
    void UpdatePoints();
    virtual void OnPointMoved(size_t pointIndex, Vector2 displacement);
    virtual void CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer);
    Rectangle GetCircleBounds(Vector2 position, float radius);
    World(int screenWidth, int screenHeight);
    virtual ~World();
};

#endif // WORLD_H
