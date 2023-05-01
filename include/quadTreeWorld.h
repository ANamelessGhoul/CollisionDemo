#ifndef QUADTREE_WORLD_H
#define QUADTREE_WORLD_H

#include "world.h"
#include "quadTree.h"

class QuadTreeWorld : public World {
public:
    QuadTreeWorld(int screenWidth, int screenHeight);
    ~QuadTreeWorld();

    virtual void OnPointMoved(size_t pointIndex, Vector2 displacement) override;
    virtual void CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer) override;
    virtual void DrawPoints() override;
private:
    Quadtree quadtree;
    std::unordered_map<size_t, Vector2> bufferPoints;
    int buffer;
    size_t pointIndicesCount; // To keep track of the number of indices stored in the array
    // Store indices for each point in the quadtree
    void InsertPointToQuadtree(size_t index);
    void RemovePointFromQuadtree(size_t index);
};

#endif // QUADTREE_WORLD_H