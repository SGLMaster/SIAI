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
        int parentRow, parentColumn; 
        // f = g + h 
        double f, g, h; 
    };

    int m_numberOfRows;
    int m_numberOfCols;

public:
    PathFinder() = delete;
    PathFinder(int numberOfRows, int numberOfCols);
    ~PathFinder();

    bool find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination);

private:
    bool isValid(int row, int column) const noexcept;
    bool isBlocked(const MapGrid& mapGrid, int row, int column) const noexcept;
    bool isDestination(int row, int column, const MapPosition& destination) const noexcept;

    double calculateHValue(int row, int column, const MapPosition& destination) const noexcept;

    void tracePath(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept;
};