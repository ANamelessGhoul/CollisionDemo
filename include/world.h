#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include <stdlib.h>
#include <vector>
#include "random.h"
#include "raymath.h"

class World
{
protected:
    unsigned long long objectCount;
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<float> radiuses;

    const float WORLD_SIZE = { 5000.0f };
    const unsigned long long DESIRED_COUNT = {100000ull};

    virtual void CreateRandomPoint();
public:
    virtual void DrawPoints();
    virtual void UpdatePoints();
    virtual void OnPointMoved(int pointIndex, Vector2 displacement);
    World(/* args */);
    virtual ~World();
};

#endif // WORLD_H
