#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include <stdlib.h>
#include <vector>
#include "random.h"
#include "raymath.h"
#include "quadTree.h"

class World
{
protected:
    unsigned long long objectCount;
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<float> radiuses;
    QuadTree quadTree;
    const float WORLD_SIZE = { 5000.0f };
    const unsigned long long DESIRED_COUNT = {100000ull};

    virtual void BuildQuadTree(Node* node);
    virtual void UpdateQuadTree();
    virtual void CreateRandomPoint();
public:
    virtual void DrawPoints();
    virtual void UpdatePoints();
    virtual void OnPointMoved(int pointIndex, Vector2 displacement);
    virtual int QueryNearest(const Vector2& point);
    World(/* args */);
    virtual ~World();
};

#endif // WORLD_H
