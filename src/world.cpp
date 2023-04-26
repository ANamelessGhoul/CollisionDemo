#include "world.h"

World::World(/* args */)
{
    objectCount = 0;
    srand(644939421);

    for (size_t i = 0; i < DESIRED_COUNT; i++)
    {
        CreateRandomPoint();
    }
}

World::~World()
{
}

void World::CreateRandomPoint()
{
    objectCount++;
    positions.emplace_back(GetRandomVector2InRange(-WORLD_SIZE, WORLD_SIZE));
    velocities.emplace_back(Vector2Scale(GetRandomDirection(), GetRandomFloatInRange(0, 100)));
    radiuses.emplace_back(GetRandomFloatInRange(5, 20));
}

void World::DrawPoints()
{
    auto screenRect = Rectangle{0, 0, 800, 450};
    for (size_t i = 0; i < objectCount; i++)
    {
        if (!CheckCollisionCircleRec(positions[i], radiuses[i], screenRect))
            continue;

        bool hasCollision = false;
        // for(size_t j = 0; j < objectCount; j++)
        // {
        //     if (j == i)
        //         continue;
        //     if (CheckCollisionCircles(positions[i], radiuses[i], positions[j], radiuses[j]))
        //     {
        //         hasCollision = true;
        //         break;
        //     }
        // }

        DrawCircleV(positions[i], radiuses[i], hasCollision ? RED : BLACK);
    }  
}

void World::UpdatePoints()
{
    float deltaTime = GetFrameTime();
    for (size_t i = 0; i < objectCount; i++)
    {
        Vector2 displacement = Vector2Scale(velocities[i], deltaTime);
        OnPointMoved(i, displacement);
        positions[i] = Vector2Add(positions[i], displacement);
    }  
}

void World::OnPointMoved(int index, Vector2 displacement)
{

}
