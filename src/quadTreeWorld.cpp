// #include "quadTreeWorld.h"
// #include "quadTree.h"

// QuadTreeWorld::QuadTreeWorld() {
//     // Determine the bounds of the world and initialize quadTree
//     Rectangle bounds = {-WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2};
//     quadTree = new Quadtree(bounds, DESIRED_COUNT); // Define an appropriate capacity value
// }


// QuadTreeWorld::~QuadTreeWorld() {
//     delete quadTree;
// }

// void QuadTreeWorld::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer) {
//     std::vector<Vector2> results;
//     quadTree->query(bounds, results);
    
//     for (const auto& point : results) {
//         auto it = std::find(positions.begin(), positions.end(), point);
//         if (it != positions.end()) {
//             buffer.push_back(std::distance(positions.begin(), it));
//         }
//     }
// }

// void QuadTreeWorld::OnPointMoved(size_t pointIndex, Vector2 displacement) {
//     // Remove the old point from the quadtree
//     quadTree->remove(positions[pointIndex]);
    
//     // Update the point's position
//     positions[pointIndex] = Vector2Add(positions[pointIndex], displacement);
    
//     // Insert the updated point into the quadtree
//     quadTree->insert(positions[pointIndex]);
// }


#include "quadTreeWorld.h"

QuadTreeWorld::QuadTreeWorld(int screenWidth, int screenHeight) : World(screenWidth, screenHeight), quadtree({-WORLD_SIZE, -WORLD_SIZE, 2 * WORLD_SIZE, 2 * WORLD_SIZE}, 16) {
    pointIndicesCount = 0; // Reset the pointIndicesCount to 0
    for (size_t i = 0; i < objectCount; ++i) {
        InsertPointToQuadtree(i);
    }
}

QuadTreeWorld::~QuadTreeWorld() {
}

void QuadTreeWorld::OnPointMoved(size_t pointIndex, Vector2 displacement) {
    RemovePointFromQuadtree(pointIndex);
    InsertPointToQuadtree(pointIndex);
}

void QuadTreeWorld::CheckCollision(const Rectangle& bounds, std::vector<size_t>& buffer) {
    pointIndicesCount = 0;
    quadtree.query(bounds, pointIndices, pointIndicesCount);
    buffer.clear();
    for (size_t i = 0; i < pointIndicesCount; ++i) {
        buffer.push_back(pointIndices[i]);
    }
}

void QuadTreeWorld::DrawPoints() {
    quadtree.query(screenRect, pointIndices, pointIndicesCount);

    for (size_t i = 0; i < pointIndicesCount; ++i) {
        Rectangle bounds = GetCircleBounds(positions[pointIndices[i]], radiuses[pointIndices[i]]);
        quadtree.query(bounds, colBuf, pointIndicesCount);

        bool hasCollision = false;
        for (size_t index : colBuf) {
            if (index == pointIndices[i]) {
                continue;
            }

            if (CheckCollisionCircles(positions[pointIndices[i]], radiuses[pointIndices[i]], positions[index], radiuses[index])) {
                hasCollision = true;
                break;
            }
        }

        DrawCircleV(positions[pointIndices[i]], radiuses[pointIndices[i]], hasCollision ? RED : BLACK);
    }
}





void QuadTreeWorld::InsertPointToQuadtree(size_t index) {
    quadtree.insert(index, positions[index]);
}

void QuadTreeWorld::RemovePointFromQuadtree(size_t index) {
    quadtree.remove(index, positions[index]);
}
