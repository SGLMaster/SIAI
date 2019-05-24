#pragma once

#include <vector>

class MapPosition;

using MapGrid = std::vector<std::vector<int>>;

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

    bool find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination);

private:
    bool isValid(int column, int row) const noexcept;
    bool isBlocked(const MapGrid& mapGrid, int column, int row) const noexcept;
    bool isDestination(int column, int row, const MapPosition& destination) const noexcept;

    double calculateHValue(int column, int row, const MapPosition& destination) const noexcept;

    void tracePath(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept;
};