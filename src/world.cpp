#include "world.h"

Rectangle GetCircleBounds(Vector2 position, float radius)
{
    return {position.x - radius, position.y - radius, radius * 2, radius * 2};
}

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

Vector2& World::GetPosition(int index)
{
    return positions[index];
}

int World::PointsSize()
{
    return positions.size();
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
    screenCollisionsBuffer.clear();
    CheckCollision(screenRect, screenCollisionsBuffer);
    for (size_t i : screenCollisionsBuffer)
    {
        collisionsBuffer.clear();
        Rectangle bounds = {positions[i].x - radiuses[i], positions[i].y - radiuses[i], radiuses[i] * 2, radiuses[i] * 2};
        CheckCollision(bounds, collisionsBuffer);
        
        bool hasCollision = false;
        for (size_t index : collisionsBuffer)
        {
            // Ignore self collisions
            if (index == i)
                continue;
            
            // Do precise collision
            if (!CheckCollisionCircles(positions[i], radiuses[i], positions[index], radiuses[index]))
                continue;

            hasCollision = true;
            break;
        }
        

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

void World::OnPointMoved(size_t index, Vector2 displacement)
{

}

void World::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer)
{
    for(size_t i = 0; i < objectCount; i++)
    {
        if(CheckCollisionCircleRec(positions[i], radiuses[i], bounds))
        {
            buffer.emplace_back(i);
        }
    }
}