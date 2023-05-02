#include "uniformGrid.h"
#include "raymath.h"
#include <algorithm>

template< typename T >
typename std::vector<T>::iterator insert_sorted( std::vector<T> * vec, T const& item ) 
{
    return vec->insert
        ( 
            std::upper_bound( vec->begin(), vec->end(), item ),
            item 
        );
}

UniformGrid::UniformGrid(int gridSize) : GRID_SIZE{gridSize}
{
}

UniformGrid::~UniformGrid()
{
    for (auto &&i : cells)
    {
        delete i.second;
    }
}

void UniformGrid::Insert(std::size_t pointIndex, const Rectangle& boundingRect)
{
    Vector2 min = {boundingRect.x, boundingRect.y};
    Vector2 max = {boundingRect.x + boundingRect.width, boundingRect.y + boundingRect.height};

    Index2D minIndex = {static_cast<int>(min.x / GRID_SIZE), static_cast<int>(min.y / GRID_SIZE)};
    Index2D maxIndex = {static_cast<int>(max.x / GRID_SIZE), static_cast<int>(max.y / GRID_SIZE)};

    for (int x = minIndex.x(); x <= maxIndex.x(); x++)
    {
        for (int y = minIndex.y(); y <= maxIndex.y(); y++)
        {
            Index2D index = {x, y};
            if (cells[index] == nullptr)
                cells[index] = new std::vector<size_t>;
            insert_sorted(cells[index], pointIndex);
        }
    }
}

void UniformGrid::Remove(std::size_t pointIndex, const Rectangle& boundingRect)
{
    Vector2 min = {boundingRect.x, boundingRect.y};
    Vector2 max = {boundingRect.x + boundingRect.width, boundingRect.y + boundingRect.height};

    Index2D minIndex = {static_cast<int>(min.x / GRID_SIZE), static_cast<int>(min.y / GRID_SIZE)};
    Index2D maxIndex = {static_cast<int>(max.x / GRID_SIZE), static_cast<int>(max.y / GRID_SIZE)};

    for (int x = minIndex.x(); x <= maxIndex.x(); x++)
    {
        for (int y = minIndex.y(); y <= maxIndex.y(); y++)
        {
            Index2D index = {x, y};
            if (cells[index] == nullptr)
                continue;
            auto& points = cells[index];
            auto first = std::lower_bound(points->begin(), points->end(), pointIndex);
            // Could not find point
            if (first == points->end())
                continue;
            points->erase(first);
        }
    }
    
}

void UniformGrid::Query(const Rectangle& rect, std::vector<std::size_t>& buffer)
{
    Vector2 min = {rect.x, rect.y};
    Vector2 max = {rect.x + rect.width, rect.y + rect.height};

    Index2D minIndex = {static_cast<int>(min.x / GRID_SIZE), static_cast<int>(min.y / GRID_SIZE)};
    Index2D maxIndex = {static_cast<int>(max.x / GRID_SIZE), static_cast<int>(max.y / GRID_SIZE)};

    const int minX = minIndex.first;
    const int minY = minIndex.second;
    const int maxX = maxIndex.first;
    const int maxY = maxIndex.second;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Index2D index = {x, y};
            if (cells[index] == nullptr)
                continue;
            auto& points = cells[index];
            for (size_t i = 0; i < points->size(); i++)
            {
                auto val = (*points)[i];
                // Don't add the same point twice
                if(std::binary_search(buffer.begin(), buffer.end(), val))
                    continue;
                
                insert_sorted(&buffer, val);
            }
        }
    }
}