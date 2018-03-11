#ifndef CITY_H
#define CITY_H
#include "Position.h"
#include "Unit.h"
#include "Building.h"
#include <vector>

class City
{
    public:
        City() {}
        virtual ~City() {}

        Position position;

        int Population = 1;

        int ProductionPerTurn = 4;

        int ProductionFromTiles = 0;

        int Production = 0;

        double ProductionModifier = 1.0;

        int FoodPerTurnFromCity = 1;

        int FoodPerTurnFromTiles = 0;

        int FoodSurplus = 0;

        int GoldPerTurnFromCity = 3;

        int GoldPerTurn = 0;

        int turnsToExpand = 5;

        std::string cityName;

        int parentIndex;

        int Health = 100;

        bool isCapital = false;

        bool isProducing = false;

        bool isProducingUnit = false;

        Unit unitBeingProduced;

        Building buildingBeingProduced;

        std::vector<std::string> AvailableBuildingsToCreate;

        std::vector<std::string> buildings;


    protected:
    private:
};

#endif // CITY_H
