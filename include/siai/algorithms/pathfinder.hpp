#pragma once

#include "map/entities/mapentity.hpp"

#include <vector>
#include <deque>

using MapGrid = std::vector<std::vector<int>>;
using MapPath = std::deque<MapPosition>;

class PathFinder
{
private:
    // A structure to hold the neccesary cell parameters 
    struct Cell
    { 
        // Row and Column index of its parent Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
        int parentColumn, parentRow; 
        // f = g + h 
        double f, g, h; 
    };

    int m_numberOfColumns;
    int m_numberOfRows;

public:
    PathFinder() = delete;
    PathFinder(int numberOfColumns, int numberOfRows);
    ~PathFinder();

    MapPath find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination);
    MapPosition getNextStep(const MapPath& path, const MapPosition& currentPosition);

private:
    bool isValid(int column, int row) const noexcept;
    bool isBlocked(const MapGrid& mapGrid, int column, int row) const noexcept;
    bool isOpposite(const MapGrid& mapGrid, const MapDirection& direction, int column, int row) const noexcept;
    bool isDestination(int column, int row, const MapPosition& destination) const noexcept;

    double calculateHValue(int column, int row, const MapPosition& destination) const noexcept;

    MapPath trace(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept;
};