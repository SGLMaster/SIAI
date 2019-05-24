#include "algorithms/pathfinder.hpp"

#include "map/entities/mapentity.hpp"

#include <set>
#include <stack>

#include <cstring>
#include <cfloat>

using CellData = std::pair< double, std::pair<int, int> >; 

PathFinder::PathFinder(int numberOfRows, int numberOfCols) : m_numberOfRows{numberOfRows}, 
                                                            m_numberOfCols{numberOfCols} {}

PathFinder::~PathFinder(){}

// A Function to find the shortest path between a given source cell to a destination cell according to 
// A* Search Algorithm 
bool PathFinder::find(const MapGrid& mapGrid, const MapPosition& source, const MapPosition& destination) 
{
    // If either the source or destination is out of range
    if(!isValid(source.row, source.column) || !isValid(destination.row, destination.column))
        return false;

    // Either the source or the destination is blocked 
    if(isBlocked(mapGrid, source.row, source.column) || isBlocked(mapGrid, destination.row, destination.column))
        return false;

    // If the destination cell is the same as source cell 
    if(isDestination(source.row, source.column, destination)) 
        return false; 

    // Create a closed list and initialise it to false which means 
    // that no cell has been included yet 
    // This closed list is implemented as a boolean 2D array 
    bool closedList[m_numberOfRows][m_numberOfCols]; 
    std::memset(closedList, false, sizeof (closedList)); 

    // Declare a 2D array of structure to hold the details 
    //of that cell 

    //Cell cells[m_numberOfRows][m_numberOfCols];

    std::vector<std::vector<Cell>> cells;
    cells.resize(m_numberOfRows);

    for(auto& row : cells)
    {
        row.resize(m_numberOfCols);
    }
  
    int row, column; 
  
    for (row=0; row<m_numberOfRows; row++) 
    { 
        for (column=0; column<m_numberOfCols; column++) 
        { 
            cells[row][column].f = FLT_MAX; 
            cells[row][column].g = FLT_MAX; 
            cells[row][column].h = FLT_MAX; 
            cells[row][column].parentRow = -1; 
            cells[row][column].parentColumn = -1; 
        } 
    } 

    // Initialising the parameters of the starting node 
    row = source.row, column = source.column; 
    cells[row][column].f = 0.0; 
    cells[row][column].g = 0.0; 
    cells[row][column].h = 0.0; 
    cells[row][column].parentRow = row; 
    cells[row][column].parentColumn = column;

    /* 
     Create an open list having information as <f, <i, j>> 
     where f = g + h, and i, j are the row and column index of that cell 
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1. This open list is implemented as a set of pair of pair.
     */
    std::set<CellData> openList; 

    // Put the starting cell on the open list and set its 'f' as 0 
    openList.insert( std::make_pair(0.0, std::make_pair (row, column)) );

    while(!openList.empty()) 
    {
        CellData curVertex = *openList.begin(); 
  
        // Remove this vertex from the open list 
        openList.erase(openList.begin()); 

        // Add this vertex to the closed list 
        row = curVertex.second.first; 
        column = curVertex.second.second; 
        closedList[row][column] = true; 

        /* 
        Generating all the 4 successor of this cell 
  
                   N       
                   |     
                   |    
            W----Cell----E 
                   |   
                   |    
                   S  
  
        Cell-->Popped Cell (i, j) 
        N -->  North       (i-1, j) 
        S -->  South       (i+1, j) 
        E -->  East        (i, j+1) 
        W -->  West           (i, j-1) 
        */
  
        // To store the 'g', 'h' and 'f' of the 4 successors 
        double gNew, hNew, fNew; 

        //----------- 1st Successor (East) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(row, column+1)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(row, column+1, destination)) 
            { 
                // Set the Parent of the destination cell 
                cells[row][column+1].parentRow = row; 
                cells[row][column+1].parentColumn = column; 
                tracePath(cells, destination);
                return true; 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[row][column+1] && !isBlocked(mapGrid, row, column+1))
            { 
                gNew = cells[row][column].g + 1.0; 
                hNew = calculateHValue(row, column+1, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[row][column+1].f == FLT_MAX || cells[row][column+1].f > fNew) 
                { 
                    openList.insert(std::make_pair(fNew, std::make_pair (row, column+1))); 
  
                    // Update the details of this cell 
                    cells[row][column+1].f = fNew; 
                    cells[row][column+1].g = gNew; 
                    cells[row][column+1].h = hNew; 
                    cells[row][column+1].parentRow = row; 
                    cells[row][column+1].parentColumn = column; 
                } 
            } 
        } 

        //----------- 2nd Successor (South) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(row+1, column)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(row+1, column, destination)) 
            { 
                // Set the Parent of the destination cell 
                cells[row+1][column].parentRow = row; 
                cells[row+1][column].parentColumn = column; 
                tracePath(cells, destination);
                return true; 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[row+1][column] && !isBlocked(mapGrid, row+1, column))
            { 
                gNew = cells[row][column].g + 1.0; 
                hNew = calculateHValue(row+1, column, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[row+1][column].f == FLT_MAX || cells[row+1][column].f > fNew) 
                { 
                    openList.insert(std::make_pair(fNew, std::make_pair (row+1, column))); 
  
                    // Update the details of this cell 
                    cells[row+1][column].f = fNew; 
                    cells[row+1][column].g = gNew; 
                    cells[row+1][column].h = hNew; 
                    cells[row+1][column].parentRow = row; 
                    cells[row+1][column].parentColumn = column; 
                } 
            } 
        }

        //----------- 3rd Successor (West) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(row, column-1)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(row, column-1, destination)) 
            { 
                // Set the Parent of the destination cell 
                cells[row][column-1].parentRow = row; 
                cells[row][column-1].parentColumn = column; 
                tracePath(cells, destination);
                return true; 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[row][column-1] && !isBlocked(mapGrid, row, column-1))
            { 
                gNew = cells[row][column].g + 1.0; 
                hNew = calculateHValue(row, column-1, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[row][column-1].f == FLT_MAX || cells[row][column-1].f > fNew) 
                { 
                    openList.insert(std::make_pair(fNew, std::make_pair (row, column-1))); 
  
                    // Update the details of this cell 
                    cells[row][column-1].f = fNew; 
                    cells[row][column-1].g = gNew; 
                    cells[row][column-1].h = hNew; 
                    cells[row][column-1].parentRow = row; 
                    cells[row][column-1].parentColumn = column; 
                } 
            } 
        } 

        //----------- 4th Successor (North) ------------ 
  
        // Only process this cell if this is a valid one 
        if(isValid(row-1, column)) 
        { 
            // If the destination cell is the same as the current successor 
            if(isDestination(row-1, column, destination)) 
            { 
                // Set the Parent of the destination cell 
                cells[row-1][column].parentRow = row; 
                cells[row-1][column].parentColumn = column; 
                tracePath(cells, destination);
                return true; 
            } 
            // If the successor is already on the closed list or if it is blocked, then ignore it. 
            // Else do the following 
            else if(!closedList[row-1][column] && !isBlocked(mapGrid, row-1, column))
            { 
                gNew = cells[row][column].g + 1.0; 
                hNew = calculateHValue(row-1, column, destination); 
                fNew = gNew + hNew; 
  
                // If it isn�t on the open list, add it to the open list. Make the current square 
                // the parent of this square. Record the f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cells[row-1][column].f == FLT_MAX || cells[row-1][column].f > fNew) 
                { 
                    openList.insert(std::make_pair(fNew, std::make_pair (row-1, column))); 
  
                    // Update the details of this cell 
                    cells[row-1][column].f = fNew; 
                    cells[row-1][column].g = gNew; 
                    cells[row-1][column].h = hNew; 
                    cells[row-1][column].parentRow = row; 
                    cells[row-1][column].parentColumn = column; 
                } 
            } 
        }

    }

    return false;
}

// A Utility Function to check whether given position is a valid cell inside the map or not. 
bool PathFinder::isValid(int row, int column) const noexcept
{
    // Returns true if row number and column number is in range 
    return (row >= 0) && (row < m_numberOfRows) && (column >= 0) && (column < m_numberOfCols); 
}

// A Utility Function to check whether the given cell is blocked or not 
bool PathFinder::isBlocked(const MapGrid& mapGrid, int row, int column) const noexcept
{
    // Returns false if the cell is blocked else true 
    if(mapGrid[row][column] == -1) 
        return true; 

    return false; 
}

// A Utility Function to check whether destination cell has been reached or not 
bool PathFinder::isDestination(int row, int column, const MapPosition& destination) const noexcept
{ 
    if (row == destination.row && column == destination.column) 
        return true; 

    return false; 
} 

// A Utility Function to calculate the 'h' heuristics. 
double PathFinder::calculateHValue(int row, int column, const MapPosition& destination) const noexcept
{ 
    // Return using the distance formula 
    return ((double) abs( row-destination.row ) + abs(column-destination.column) ); 
} 

// A Utility Function to trace the path from the source 
// to destination 
void PathFinder::tracePath(const std::vector<std::vector<Cell>>& cells, const MapPosition& destination) const noexcept 
{  
    int row = destination.row; 
    int column = destination.column; 
  
    std::stack<std::pair<int, int>> Path; 
  
    while (!(cells[row][column].parentRow == row && cells[row][column].parentColumn == column )) 
    { 
        Path.push(std::make_pair(row, column)); 
        int temp_row = cells[row][column].parentRow; 
        int temp_column = cells[row][column].parentColumn; 
        row = temp_row; 
        column = temp_column; 
    } 
  
    Path.push(std::make_pair(row, column)); 
    while(!Path.empty())
    {
        std::pair<int, int> curCell = Path.top();
        Path.pop();
        printf(" -> (%d, %d) ", curCell.first, curCell.second);
    }
} 

int main() 
{
    PathFinder myFinder(5, 5);

    std::vector<int> firstRow  { 0,  0,  0,  0,  0};
    std::vector<int> secondRow {-1, -1, -1,  1, -1};
    std::vector<int> thirdRow  { 3, -1, -1,  1, -1};
    std::vector<int> fourthRow { 3, -1, -1,  1, -1};
    std::vector<int> fifthRow  { 2,  2,  2,  1, -1};

    MapGrid grid;
    grid.push_back(firstRow);
    grid.push_back(secondRow);
    grid.push_back(thirdRow);
    grid.push_back(fourthRow);
    grid.push_back(fifthRow);

    MapPosition source{0, 0};
    MapPosition destination{0, 2};

    bool pathFound = myFinder.find(grid, source, destination);

    if(pathFound)
        printf("Found it! :)");
    else
        printf("I'm lost! :(");

    return 0; 
}