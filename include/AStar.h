#ifndef ASTAR_H
#define ASTAR_H
#include <utility>
#include "Unit.h"

#define WORLDSIZE 50

typedef std::pair<int, int> position;

typedef std::pair<double, std::pair<int, int>> positionFValue;

struct cell
{

    int parentX;
    int parentY;

    double f, g, h;

};

namespace AStar {

        bool isValid (int row, int column);
        bool isDestination (int row, int col, position dest);

        int returnTerrainMovementCost (int civilizationIndexrid[WORLDSIZE][WORLDSIZE*4], int row, int col, Unit &Unit);

        double calculateHValue (int row, int col, position dest);

        void tracePath(cell cellDetails[WORLDSIZE][WORLDSIZE*4], position dest, Unit &Unit);

        extern void aStarSearch (int civilizationIndexrid[WORLDSIZE][WORLDSIZE*4], position src, position dest, Unit &Unit);

};

#endif // ASTAR_H
