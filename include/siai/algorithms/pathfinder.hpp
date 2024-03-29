#pragma once

#include "map/common.hpp"

#include <vector>

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

    MapPath find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination) const noexcept;
    static MapPosition getNextStep(const MapPath& path, const MapPosition& currentPosition);
    static MapDirection getNextDirection(const MapPosition& currentPosition, const MapPosition& nextPosition);

private:
    bool isValid(int column, int row) const noexcept;
    bool isBlocked(const MapGrid& mapGrid, int column, int row) const noexcept;
    bool isOpposite(const MapGrid& mapGrid, const MapDirection& direction, int column, int row) const noexcept;
    bool isDestination(int column, int row, const MapPosition& destination) const noexcept;

    double calculateHValue(int column, int row, const MapPosition& destination) const noexcept;

    MapPath trace(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept;
};