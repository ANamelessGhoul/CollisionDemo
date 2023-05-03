#ifndef UNIFORM_GRID_WORLD_H
#define UNIFORM_GRID_WORLD_H

#include "world.h"
#include "uniformGrid.h"

class UniformGridWorld : public World
{
private:
    UniformGrid uniformGrid;
public:
    UniformGridWorld();
    ~UniformGridWorld();

    void Draw() override;
    void CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer) override;
    void OnPointAdded(size_t pointIndex) override;
    void OnPointMoved(size_t pointIndex, Vector2 displacement) override;

};

#endif // UNIFORM_GRID_WORLD_H