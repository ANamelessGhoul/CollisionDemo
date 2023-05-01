#ifndef UNIFORM_GRID_H
#define UNIFORM_GRID_H

#include <unordered_map>
#include <vector>
#include "raylib.h"
#include <memory>

class UniformGrid
{
private:
    struct Index2D : std::pair<int, int>{
        Index2D(int x, int y): std::pair<int, int>(x, y) {};
        int x() const { return first; };
        int y() const { return second; };
    };
    
    struct Index2DHash 
    {
        std::size_t operator()(const Index2D& k) const {
            // simple hash function
            return ((std::hash<int>()(k.x()) ^ (std::hash<int>()(k.y()) << 1)) >> 1);
        }
    };

    const int GRID_SIZE;

    std::unordered_map<Index2D, std::vector<std::size_t>*, Index2DHash> cells;

public:
    UniformGrid(int gridSize);
    ~UniformGrid();

    void Insert(std::size_t pointIndex, const Rectangle& boundingRect);
    void Remove(std::size_t pointIndex, const Rectangle& boundingRect);
    void Query(const Rectangle& rect, std::vector<std::size_t>& buffer);
};




#endif // UNIFORM_GRID_H
