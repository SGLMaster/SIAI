#include "algorithms/pathfinder.hpp"

#include <set>

#include <cstring>
#include <cfloat>

using CellData = std::pair< double, std::pair<int, int> >; 

PathFinder::PathFinder(int numberOfColumns, int numberOfRows) : m_numberOfColumns{numberOfColumns}, 
                                                                m_numberOfRows{numberOfRows} {}

PathFinder::~PathFinder(){}

// A Function to find the shortest path between a given source cell to a destination cell according to 
// A* Search Algorithm 
MapPath PathFinder::find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination) 
{
    MapPath emptyPath;

    // If either the source or destination is out of range
    if(!isValid(source.column, source.row) || !isValid(destination.column, destination.row))
        return emptyPath;

    // Either the source or the destination is blocked 
    if(isBlocked(mapGrid, source.column, source.row) || isBlocked(mapGrid, destination.column, destination.row))
        return emptyPath;

    // If the destination cell is the same as source cell 
    if(isDestination(source.column, source.row, destination)) 
        return emptyPath; 

    // Create a closed list and initialise it to false which means 
    // that no cell has been included yet 
    // This closed list is implemented as a boolean 2D array 
    bool closedList[m_numberOfColumns][m_numberOfRows]; 
    std::memset(closedList, false, sizeof (closedList)); 

    // Declare a 2D array of structure to hold the details 
    //of that cell 

    //Cell cells[m_numberOfRows][m_numberOfCols];

    std::vector<std::vector<Cell>> cells;
    cells.resize(m_numberOfColumns);

    for(auto& column : cells)
    {
        column.resize(m_numberOfRows);
    }
  
    int column, row; 
  
    for (column=0; column<m_numberOfColumns; column++) 
    { 
        for (row=0; row<m_numberOfRows; row++) 
        { 
            cells[column][row].f = FLT_MAX; 
            cells[column][row].g = FLT_MAX; 
            cells[column][row].h = FLT_MAX; 
            cells[column][row].parentColumn = -1;
            cells[column][row].parentRow = -1;  
        } 
    } 

    // Initialising the parameters of the starting node 
    column = source.column, row = source.row; 
    cells[column][row].f = 0.0; 
    cells[column][row].g = 0.0; 
    cells[column][row].h = 0.0; 
    cells[column][row].parentColumn = column;
    cells[column][row].parentRow = row; 

    /* 
     Create an open list having information as <f, <i, j>> 
     where f = g + h, and i, j are the row and column index of that cell 
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1. This open list is implemented as a set of pair of pair.
     */
    std::set<CellData> openList; 

    // Put the starting cell on the open list and set its 'f' as 0 
    openList.insert( std::make_pair(0.0, std::make_pair (column, row)) );

    while(!openList.empty()) 
    {
        CellData curVertex = *openList.begin(); 
  
        // Remove this vertex from the open list 
        openList.erase(openList.begin()); 

        // Add this vertex to the closed list  
        column = curVertex.second.first; 
        row = curVertex.second.second;
        closedList[column][row] = true; 

        /* 
        Generating all the 4 successor of this cell 
  
                   U       
                   |     
                   |    
            L----Cell----R 
                   |   
                   |    
                   D  
  
        Cell-->Popped Cell (i, j) 
        U -->  Up          (i-1, j) 
        D -->  Down        (i+1, j) 
        R -->  Right       (i, j+1) 
        L -->  Left        (i, j-1) 
        */
  
        // To store the 'g', 'h' and 'f' of the 4 successors 
        double gNew, hNew, fNew; 

        //----------- 1st Successor (Right) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(column+1, row)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(column+1, row, destination)
                && !isOpposite(mapGrid, MapDirection::RIGHT, column+1, row)) 
            { 
                // Set the Parent of the destination cell 
                cells[column+1][row].parentColumn = column; 
                cells[column+1][row].parentRow = row; 
                return trace(cells, destination); 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[column+1][row] && !isBlocked(mapGrid, column+1, row) 
                    && !isOpposite(mapGrid, MapDirection::RIGHT, column+1, row))
            { 
                gNew = cells[column][row].g + 1.0; 
                hNew = calculateHValue(column+1, row, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[column+1][row].f == FLT_MAX || cells[column+1][row].f > fNew) 
                { 
                    openList.insert(std::make_pair( fNew, std::make_pair(column+1, row) )); 
  
                    // Update the details of this cell 
                    cells[column+1][row].f = fNew; 
                    cells[column+1][row].g = gNew; 
                    cells[column+1][row].h = hNew; 
                    cells[column+1][row].parentColumn = column; 
                    cells[column+1][row].parentRow = row; 
                } 
            } 
        } 

        //----------- 2nd Successor (Down) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(column, row+1)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(column, row+1, destination)
                && !isOpposite(mapGrid, MapDirection::DOWN, column, row+1)) 
            { 
                // Set the Parent of the destination cell 
                cells[column][row+1].parentColumn = column;
                cells[column][row+1].parentRow = row;  
                return trace(cells, destination); 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[column][row+1] && !isBlocked(mapGrid, column, row+1)
                    && !isOpposite(mapGrid, MapDirection::DOWN, column, row+1))
            { 
                gNew = cells[column][row].g + 1.0; 
                hNew = calculateHValue(column, row+1, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[column][row+1].f == FLT_MAX || cells[column][row+1].f > fNew) 
                { 
                    openList.insert(std::make_pair( fNew, std::make_pair(column, row+1) )); 
  
                    // Update the details of this cell 
                    cells[column][row+1].f = fNew; 
                    cells[column][row+1].g = gNew; 
                    cells[column][row+1].h = hNew; 
                    cells[column][row+1].parentColumn = column; 
                    cells[column][row+1].parentRow = row; 
                } 
            } 
        }

        //----------- 3rd Successor (Left) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(column-1, row)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(column-1, row, destination)
                && !isOpposite(mapGrid, MapDirection::LEFT, column-1, row)) 
            { 
                // Set the Parent of the destination cell 
                cells[column-1][row].parentColumn = column; 
                cells[column-1][row].parentRow = row; 
                return trace(cells, destination); 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[column-1][row] && !isBlocked(mapGrid, column-1, row) 
                    && !isOpposite(mapGrid, MapDirection::LEFT, column-1, row))
            { 
                gNew = cells[column][row].g + 1.0; 
                hNew = calculateHValue(column-1, row, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[column-1][row].f == FLT_MAX || cells[column-1][row].f > fNew) 
                { 
                    openList.insert(std::make_pair( fNew, std::make_pair(column-1, row) )); 
  
                    // Update the details of this cell 
                    cells[column-1][row].f = fNew; 
                    cells[column-1][row].g = gNew; 
                    cells[column-1][row].h = hNew; 
                    cells[column-1][row].parentColumn = column; 
                    cells[column-1][row].parentRow = row; 
                } 
            } 
        } 

        //----------- 4th Successor (Up) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(column, row-1)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(column, row-1, destination)
                && !isOpposite(mapGrid, MapDirection::UP, column, row-1)) 
            { 
                // Set the Parent of the destination cell 
                cells[column][row-1].parentColumn = column;
                cells[column][row-1].parentRow = row;  
                return trace(cells, destination); 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[column][row-1] && !isBlocked(mapGrid, column, row-1) 
                    && !isOpposite(mapGrid, MapDirection::UP, column, row-1))
            { 
                gNew = cells[column][row].g + 1.0; 
                hNew = calculateHValue(column, row-1, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[column][row-1].f == FLT_MAX || cells[column][row-1].f > fNew) 
                { 
                    openList.insert(std::make_pair( fNew, std::make_pair(column, row-1) )); 
  
                    // Update the details of this cell 
                    cells[column][row-1].f = fNew; 
                    cells[column][row-1].g = gNew; 
                    cells[column][row-1].h = hNew; 
                    cells[column][row-1].parentColumn = column; 
                    cells[column][row-1].parentRow = row; 
                } 
            } 
        }

    }

    return emptyPath;
}

// A Function to find the next step in a path previously calculated 
MapPosition PathFinder::getNextStep(const MapPath& path, const MapPosition& currentPosition) 
{
    bool returnNextOne = false;

    for(auto& position : path)
    {
        if(returnNextOne)
            return position;

        if(position.column == currentPosition.column && position.row == currentPosition.row)
            returnNextOne = true;
    }

    MapPosition nullPosition{-1, -1};
    return nullPosition;
}

// A Utility Function to check whether given position is a valid cell inside the map or not. 
bool PathFinder::isValid(int column, int row) const noexcept
{
    // Returns true if row number and column number is in range 
    return (column >= 0) && (column < m_numberOfColumns) && (row >= 0) && (row < m_numberOfRows); 
}

// A Utility Function to check whether the given cell is blocked or not 
bool PathFinder::isBlocked(const MapGrid& mapGrid, int column, int row) const noexcept
{
    // Returns true if the cell is blocked else false 
    if(mapGrid[column][row] == -1) 
        return true; 

    return false; 
}

// A Utility Function to check whether the given cell has an opposite direction to the movement or not
bool PathFinder::isOpposite(const MapGrid& mapGrid, const MapDirection& direction, int column, int row) const noexcept
{
    // Returns false if the cell is faced in opposite direction to the PathFinder movement else true 
    if(mapGrid[column][row] == static_cast<int>(MapDirection::RIGHT) && direction == MapDirection::LEFT) 
        return true; 
    if(mapGrid[column][row] == static_cast<int>(MapDirection::DOWN) && direction == MapDirection::UP) 
        return true; 
    if(mapGrid[column][row] == static_cast<int>(MapDirection::LEFT) && direction == MapDirection::RIGHT) 
        return true; 
    if(mapGrid[column][row] == static_cast<int>(MapDirection::UP) && direction == MapDirection::DOWN) 
        return true; 

    return false; 
}

// A Utility Function to check whether destination cell has been reached or not 
bool PathFinder::isDestination(int column, int row, const MapPosition& destination) const noexcept
{ 
    if (column == destination.column && row == destination.row) 
        return true; 

    return false; 
} 

// A Utility Function to calculate the 'h' heuristics. 
double PathFinder::calculateHValue(int column, int row, const MapPosition& destination) const noexcept
{ 
    // Return using the distance formula 
    return ((double) abs(column-destination.column) + abs( row-destination.row ) ); 
} 

// A Utility Function to trace the path from the source 
// to destination 
MapPath PathFinder::trace(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept 
{  
    int column = destination.column;
    int row = destination.row;  
  
    MapPath path; 
  
    while( !(cells[column][row].parentColumn == column && cells[column][row].parentRow == row) ) 
    { 
        path.push_front(MapPosition{column, row}); 
        int tmpColumn = cells[column][row].parentColumn; 
        int tmpRow = cells[column][row].parentRow; 
        column = tmpColumn;
        row = tmpRow;  
    } 
  
    path.push_front(MapPosition{column, row}); 

    return path;
} 

int main() 
{
    constexpr int BLKD = -1;
    constexpr int RIGHT = static_cast<int>(MapDirection::RIGHT);
    constexpr int DOWN = static_cast<int>(MapDirection::DOWN);
    constexpr int LEFT = static_cast<int>(MapDirection::LEFT);
    constexpr int UP = static_cast<int>(MapDirection::UP);

    PathFinder myFinder(5, 7);

    std::vector<int> fifthCol  { RIGHT,  DOWN,  DOWN,   DOWN,  DOWN,   DOWN,  LEFT };
    std::vector<int> fourthCol { RIGHT,  BLKD,  LEFT,   BLKD,  RIGHT,  BLKD,  LEFT };
    std::vector<int> thirdCol  { RIGHT,  BLKD,  LEFT,   BLKD,  RIGHT,  BLKD,  LEFT };
    std::vector<int> secondCol { RIGHT,  BLKD,  LEFT,   BLKD,  RIGHT,  BLKD,  LEFT };
    std::vector<int> firstCol  { RIGHT,  UP,    UP,     UP,    UP,     UP,    LEFT };

    MapGrid grid;
    grid.push_back(firstCol);
    grid.push_back(secondCol);
    grid.push_back(thirdCol);
    grid.push_back(fourthCol);
    grid.push_back(fifthCol);

    MapPosition source{0, 0};
    MapPosition destination{2, 4};

    MapPath pathFound = myFinder.find(grid, source, destination);

    if(pathFound.empty())
        printf("I'm lost! :(\n");
    else
        printf("Found it! :)\n");

    for(const auto& curCell : pathFound)
    {
        printf(" -> (%d, %d) ", curCell.column, curCell.row);
    }

    MapPosition currentPosition = MapPosition{0, 0};
    printf("\nCurrent Position: (%d, %d)\n", currentPosition.column, currentPosition.row);

    MapPosition nextStep = myFinder.getNextStep(pathFound, currentPosition);
    printf("Next Step: (%d, %d)\n", nextStep.column, nextStep.row);

    return 0; 
}