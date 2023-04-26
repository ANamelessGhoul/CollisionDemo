#include "world.h"

World::World(/* args */) : quadTree(BoundingBoxCustom{ Vector2{ -WORLD_SIZE/2, -WORLD_SIZE/2 }, WORLD_SIZE/2, WORLD_SIZE/2 })
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
    // auto screenRect = Rectangle{0, 0, 800, 450};

    

    // for (size_t i = 0; i < objectCount; i++)
    // {
    //     if (!CheckCollisionCircleRec(positions[i], radiuses[i], screenRect))
    //         continue;

    //     bool hasCollision = false;
    //     // for(size_t j = 0; j < objectCount; j++)
    //     // {
    //     //     if (j == i)
    //     //         continue;
    //     //     if (CheckCollisionCircles(positions[i], radiuses[i], positions[j], radiuses[j]))
    //     //     {
    //     //         hasCollision = true;
    //     //         break;
    //     //     }
    //     // }

    //     DrawCircleV(positions[i], radiuses[i], hasCollision ? RED : BLACK);
    // }  

    auto screenRect = Rectangle{0, 0, 800, 450};
    std::vector<int> pointIndices;
    quadTree.search(BoundingBoxCustom{Vector2{screenRect.x + screenRect.width/2, screenRect.y + screenRect.height/2},
        screenRect.width/2, screenRect.height/2}, pointIndices);
    for (const auto& i : pointIndices)
    {
        bool hasCollision = false;
        for (const auto& j : pointIndices)
        {
            if (i == j)
                continue;
            if (CheckCollisionCircles(positions[i], radiuses[i], positions[j], radiuses[j]))
            {
                hasCollision = true;
                break;
            }
        }
        DrawCircleV(positions[i], radiuses[i], hasCollision ? RED : BLACK);
    }  
}

int World::QueryNearest(const Vector2& point)
{
    std::vector<int> results;
    float radius = 0.0f;
    Vector2 queryPoint = point;

    // Keep searching with an expanding radius until a point is found
    while (results.empty() && radius <= WORLD_SIZE) {
        BoundingBoxCustom searchBbox{ queryPoint, radius, radius };
        quadTree.search(searchBbox, results);
        radius += 10.0f;
    }

    // Find the closest point from the results
    int closestIndex = -1;
    float closestDist = FLT_MAX;
    for (const auto& index : results) {
        float dist = Vector2Distance(point, positions[index]);
        if (dist < closestDist) {
            closestDist = dist;
            closestIndex = index;
        }
    }

    return closestIndex;
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
    UpdateQuadTree();
}

void World::OnPointMoved(int index, Vector2 displacement)
{

}


void World::BuildQuadTree(Node* node) {
    for (unsigned int i = 0; i < positions.size(); i++) {
        quadTree.insert(i, positions[i]);
    }
}

void World::UpdateQuadTree() {
    for (unsigned int i = 0; i < positions.size(); i++) {
        quadTree.insert(i, positions[i]);
    }
}