#include "AStar.h"
#include "Unit.h"
#include <math.h>
#include <stack>
#include <set>
#include <string.h>
#include <cfloat>
#include <iostream>

AStar::AStar()
{
    //ctor
}

AStar::~AStar()
{
    //dtor
}

bool AStar::isValid(int row, int col)
{

    return (row >= 0) && (row < worldSize) &&
           (col >= 0) && (col < worldSize*4);

}

bool AStar::isDestination(int row, int col, position dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

double AStar::calculateHValue(int row, int col, position dest)
{

    return ((double)sqrt ((row-dest.first)*(row-dest.first)
                          + (col-dest.second)*(col-dest.second)));
}

void AStar::tracePath(cell cellDetails[worldSize][worldSize*4], position dest)
{
    int row = dest.first;
    int col = dest.second;

    std::stack<position> Path;

    while (!(cellDetails[row][col].parentX == row
             && cellDetails[row][col].parentY == col ))
    {
        Path.push (std::make_pair (row, col));
        int temp_row = cellDetails[row][col].parentX;
        int temp_col = cellDetails[row][col].parentY;
        row = temp_row;
        col = temp_col;
    }

    Path.push (std::make_pair (row, col));
    while (!Path.empty())
    {
        std::pair<int,int> p = Path.top();
        Path.pop();

    }

    return;
}

void AStar::aStarSearch(int grid[worldSize][worldSize*4], position src, position dest)
{

    bool closedList[worldSize][worldSize*4];
    memset(closedList, false, sizeof (closedList));

    cell cellDetails[worldSize][worldSize*4];

    int i, j;

    for (i = 0; i < worldSize; i++)
    {
        for (j = 0; j < worldSize*4; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parentX = -1;
            cellDetails[i][j].parentY = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parentX = i;
    cellDetails[i][j].parentY = j;

    std::set<positionFValue> openList;

    openList.insert(std::make_pair (0.0, std::make_pair (i, j)));

    bool foundDest = false;

    while (!openList.empty())
    {
        positionFValue p = *openList.begin();

        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        if (isValid(i-1, j) == true)
        {

            if (isDestination(i-1, j, dest) == true)
            {
                cellDetails[i-1][j].parentX = i;
                cellDetails[i-1][j].parentY = j;
                tracePath (cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i-1][j] == false)
            {
                gNew = cellDetails[i][j].g + grid[i-1][j];
                hNew = calculateHValue (i-1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i-1][j].f == FLT_MAX ||
                        cellDetails[i-1][j].f > fNew)
                {
                    openList.insert( std::make_pair(fNew,
                                               std::make_pair(i-1, j)));

                    cellDetails[i-1][j].f = fNew;
                    cellDetails[i-1][j].g = gNew;
                    cellDetails[i-1][j].h = hNew;
                    cellDetails[i-1][j].parentX = i;
                    cellDetails[i-1][j].parentY = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        if (isValid(i+1, j) == true)
        {

            if (isDestination(i+1, j, dest) == true)
            {
                cellDetails[i+1][j].parentX = i;
                cellDetails[i+1][j].parentY = j;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i+1][j] == false)
            {
                gNew = cellDetails[i][j].g + grid[i+1][j];
                hNew = calculateHValue(i+1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i+1][j].f == FLT_MAX ||
                        cellDetails[i+1][j].f > fNew)
                {
                    openList.insert( std::make_pair (fNew, std::make_pair (i+1, j)));
                    cellDetails[i+1][j].f = fNew;
                    cellDetails[i+1][j].g = gNew;
                    cellDetails[i+1][j].h = hNew;
                    cellDetails[i+1][j].parentX = i;
                    cellDetails[i+1][j].parentY = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        if (isValid (i, j+1) == true)
        {

            if (isDestination(i, j+1, dest) == true)
            {
                cellDetails[i][j+1].parentX = i;
                cellDetails[i][j+1].parentY = j;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }


            else if (closedList[i][j+1] == false)
            {
                gNew = cellDetails[i][j].g + grid[i][j+1];
                hNew = calculateHValue (i, j+1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j+1].f == FLT_MAX ||
                        cellDetails[i][j+1].f > fNew)
                {
                    openList.insert( std::make_pair(fNew,
                                        std::make_pair (i, j+1)));

                    cellDetails[i][j+1].f = fNew;
                    cellDetails[i][j+1].g = gNew;
                    cellDetails[i][j+1].h = hNew;
                    cellDetails[i][j+1].parentX = i;
                    cellDetails[i][j+1].parentY = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        if (isValid(i, j-1) == true)
        {

            if (isDestination(i, j-1, dest) == true)
            {
                cellDetails[i][j-1].parentX = i;
                cellDetails[i][j-1].parentY = j;
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i][j-1] == false)
            {
                gNew = cellDetails[i][j].g + grid[i][j-1];
                hNew = calculateHValue(i, j-1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i][j-1].f == FLT_MAX ||
                        cellDetails[i][j-1].f > fNew)
                {
                    openList.insert( std::make_pair (fNew,
                                          std::make_pair (i, j-1)));

                    cellDetails[i][j-1].f = fNew;
                    cellDetails[i][j-1].g = gNew;
                    cellDetails[i][j-1].h = hNew;
                    cellDetails[i][j-1].parentX = i;
                    cellDetails[i][j-1].parentY = j;
                }
            }
        }

    }



    if (foundDest == false)
        std::cout << "Failed to find destination cell!" << std::endl;

    return;
}
