#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "random.h"
#include "raylib.h"
#include "raymath.h"

class World
{
private:
    void CreateRandomPoint();
    
protected:
    unsigned long long objectCount;
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<float> radiuses;

    std::vector<size_t> screenCollisionsBuffer;
    std::vector<size_t> collisionsBuffer;

    const float WORLD_SIZE = { 2000.0f };
    const unsigned long long DESIRED_COUNT = {10000ull};

    static Rectangle GetCircleBounds(Vector2 position, float radius);

public:
    void DrawPoints();
    void UpdatePoints();
    virtual void OnPointAdded(size_t pointIndex);
    virtual void OnPointMoved(size_t pointIndex, Vector2 displacement);
    virtual void CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer);
    World(/* args */);
    virtual ~World();
};

#endif // WORLD_H
