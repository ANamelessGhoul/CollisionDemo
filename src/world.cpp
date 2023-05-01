#include "world.h"
#include <iostream>

Rectangle World::GetCircleBounds(Vector2 position, float radius)
{
    return {position.x - radius, position.y - radius, radius * 2, radius * 2};
}

World::World(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), screenRect{0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)} {
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
    screenCollisionsBuffer.clear();
    CheckCollision(screenRect, screenCollisionsBuffer);

    // Create a temporary buffer for storing indices of potential collisions
    std::vector<size_t> potentialCollisions;

    for (size_t i : screenCollisionsBuffer)
    {
        collisionsBuffer.clear();
        Rectangle bounds = GetCircleBounds(positions[i], radiuses[i]);

        // Find potential collisions only once per circle on the screen
        potentialCollisions.clear();
        CheckCollision(bounds, potentialCollisions);

        bool hasCollision = false;
        for (size_t index : potentialCollisions)
        {
            // Ignore self collisions
            if (index == i)
                continue;

            // Do precise collision
            if (CheckCollisionCircles(positions[i], radiuses[i], positions[index], radiuses[index]))
            {
                hasCollision = true;
                break;
            }
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
    buffer.reserve(objectCount);
    for(size_t i = 0; i < objectCount; i++)
    {
        if(CheckCollisionCircleRec(positions[i], radiuses[i], bounds))
        {
            buffer.push_back(i);
        }
    }
}