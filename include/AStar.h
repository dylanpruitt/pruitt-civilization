#ifndef ASTAR_H
#define ASTAR_H
#include <utility>

#define worldSize 50

typedef std::pair<int, int> position;

typedef std::pair<double, std::pair<int, int>> positionFValue;

struct cell
{

    int parentX;
    int parentY;

    double f, g, h;

};

class AStar
{
    public:
        AStar();
        virtual ~AStar();

    protected:
    private:
        bool isValid (int row, int column);
        bool isDestination (int row, int col, position dest);
        double calculateHValue (int row, int col, position dest);
        void tracePath(cell cellDetails[worldSize][worldSize*4], position dest);
        void aStarSearch (int MovementCostGrid[worldSize][worldSize*4], position src, position dest);
};

#endif // ASTAR_H
