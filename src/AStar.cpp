#include "AStar.h"
#include "Unit.h"
#include <math.h>
#include <stack>
#include <set>
#include <string.h>
#include <cfloat>
#include <iostream>
#include <algorithm>

namespace AStar {

bool isValid(int row, int col)
{

    return (row >= 0) && (row < WORLDSIZE) &&
           (col >= 0) && (col < WORLDSIZE*4);

}

bool isDestination(int row, int col, position dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

int returnTerrainMovementCost (int grid[WORLDSIZE][WORLDSIZE*4], int row, int col, Unit &unit) {

    int featureType = grid[row][col]; int CANTMOVEONTILECOST = 100000;

    switch (featureType) {

        case 0: {

            if (unit.domain.canCrossOceans) {

                return 1;

            } else {

                return CANTMOVEONTILECOST;

            }
        } break;

        case 1: {

            if (unit.domain.canCoastalEmbark) {

                return 1;

            } else {

                return CANTMOVEONTILECOST;

            }

        } break;

        case 2:
            return 2;
        break;

        case 3:
            return 2;
        break;

        case 6:
            return 2;
        break;

        case 7:
            return 2;
        break;

        default:
            return 1;
        break;
    }

}

double calculateHValue(int row, int col, position dest)
{

    return ((double)sqrt ((row-dest.first)*(row-dest.first)
                          + (col-dest.second)*(col-dest.second)));
}

void tracePath(cell cellDetails[WORLDSIZE][WORLDSIZE*4], position dest, Unit &unit)
{
    int row = dest.first;
    int col = dest.second;

    unit.moveDirectionQueue.clear();

    while (!(unit.position.x == row
             && unit.position.y == col ))
    {
        int temp_row = cellDetails[row][col].parentX;
        int temp_col = cellDetails[row][col].parentY;

        /// Direction logic

        int deltaX = temp_row - row;
        int deltaY = temp_col - col;

        int direction = 0;

        /// 1=north,2=west,3=south,4=east
        if (deltaX == -1 && deltaY == 0) { direction = 1; }
        if (deltaX == 0 && deltaY == -1) { direction = 2; }
        if (deltaX == 1 && deltaY == 0) { direction = 3; }
        if (deltaX == 0 && deltaY == 1) { direction = 4; }

        /// Make sure Direction number is 1-4
        if (direction >= 1 && direction <= 4) {

            unit.moveDirectionQueue.push_back(direction);

        }

        row = temp_row;
        col = temp_col;

    }

    std::reverse (unit.moveDirectionQueue.begin(), unit.moveDirectionQueue.end());

    return;
}

void aStarSearch(int grid[WORLDSIZE][WORLDSIZE*4], position src, position dest, Unit &unit)
{

    bool closedList[WORLDSIZE][WORLDSIZE*4];
    memset(closedList, false, sizeof (closedList));

    cell cellDetails[WORLDSIZE][WORLDSIZE*4];

    int i, j;

    for (i = 0; i < WORLDSIZE; i++)
    {
        for (j = 0; j < WORLDSIZE*4; j++)
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
                tracePath (cellDetails, dest, unit);
                foundDest = true;
                return;
            }

            else if (closedList[i-1][j] == false)
            {
                gNew = cellDetails[i][j].g + returnTerrainMovementCost (grid, i-1, j, unit);
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
                tracePath(cellDetails, dest, unit);
                foundDest = true;
                return;
            }

            else if (closedList[i+1][j] == false)
            {
                gNew = cellDetails[i][j].g + returnTerrainMovementCost (grid, i+1, j, unit);
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
                tracePath(cellDetails, dest, unit);
                foundDest = true;
                return;
            }


            else if (closedList[i][j+1] == false)
            {
                gNew = cellDetails[i][j].g + returnTerrainMovementCost (grid, i, j+1, unit);
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
                tracePath(cellDetails, dest, unit);
                foundDest = true;
                return;
            }

            else if (closedList[i][j-1] == false)
            {
                gNew = cellDetails[i][j].g + returnTerrainMovementCost (grid, i, j-1, unit);
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


}

