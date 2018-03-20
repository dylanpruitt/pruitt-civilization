#include "GameUpdater.h"
#include "sharedMethods.h"

#include <math.h>
#include <iostream>

GameUpdater::GameUpdater()
{
    //ctor
}

GameUpdater::~GameUpdater()
{
    //dtor
}

void GameUpdater::updateResources (int civilizationIndex, GameVariables &gameVariables) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldResourceMap[i][j].ResourceCode != 0 && gameVariables.worldMap.WorldTerritoryMap[i][j] == civilizationIndex+1) {

                gameVariables.Civilizations[civilizationIndex].resources.push_back(gameVariables.worldMap.WorldResourceMap[i][j]);

            }

        }

    }

}

void GameUpdater::updateCities (GameVariables &gameVariables) {

    for (int w = 0; w < gameVariables.Cities.size(); w++) {

        gameVariables.Cities[w].FoodSurplus += ((gameVariables.Cities[w].FoodPerTurnFromCity + gameVariables.Cities[w].FoodPerTurnFromTiles) - (gameVariables.Cities[w].Population*2));

        gameVariables.Cities[w].turnsToExpand -= gameVariables.Civilizations[gameVariables.Cities[w].parentIndex].ExpansionRate;

        if (gameVariables.Cities[w].turnsToExpand <= 0) {

            expandCityTerritory(w, gameVariables);

        }

        if (gameVariables.Cities[w].FoodSurplus >= (pow((gameVariables.Cities[w].Population - 1), 2) + 5)) {
            gameVariables.Cities[w].FoodSurplus -= pow((gameVariables.Cities[w].Population - 1), 2) + 5;
            gameVariables.Cities[w].Population++;
            gameVariables.Civilizations[gameVariables.Cities[w].parentIndex].Happiness -= 2;
            sharedMethods::assignWorkByPopulation(w, false, gameVariables);

        }

        if (gameVariables.Cities[w].FoodSurplus < 0) {
            gameVariables.Cities[w].Population--;
            sharedMethods::assignWorkByPopulation(w, false, gameVariables);

        }

        updateCityProductionModifier (w, gameVariables.Cities[w].parentIndex, gameVariables);

        if (gameVariables.Cities[w].isProducingUnit == true) {

            updateCityUnitProduction (w, gameVariables.Cities[w].parentIndex, gameVariables);

        } else {

            updateCityBuildingProduction (w, gameVariables.Cities[w].parentIndex, gameVariables);

        }

    }

}

bool GameUpdater::canCityExpand (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {


    for (int i = -3; i < 4; i++) {

        for (int j = -3; j < 4; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] == 0
            && sharedMethods::isTileBorderingCivilizationTerritory(gameVariables.Cities[cityIndex].position.x+i,
            gameVariables.Cities[cityIndex].position.y+j,
            civilizationIndex, gameVariables)) {

                return true;

            }

        }

    }

    return false;

}

void GameUpdater::expandCityTerritory (int cityIndex, GameVariables &gameVariables) {

    if (canCityExpand(cityIndex, gameVariables.Cities[cityIndex].parentIndex, gameVariables)) {

        bool tileFound = false;

        while (!tileFound) {

            int i = rand() % 6 - 3;
            int j = rand() % 6 - 3;

            if (gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] == 0
                && sharedMethods::isTileBorderingCivilizationTerritory(gameVariables.Cities[cityIndex].position.x+i,
                gameVariables.Cities[cityIndex].position.y+j,
                gameVariables.Cities[cityIndex].parentIndex, gameVariables)) {

                gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] = gameVariables.Cities[cityIndex].parentIndex + 1;

                tileFound = true;

            }

        }

    }

    gameVariables.Cities[cityIndex].turnsToExpand = 5;

}

void GameUpdater::updateCityProductionModifier (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    double tempModifier = 100;

    if (gameVariables.Civilizations[civilizationIndex].Happiness >= 70) {

        double happinessModifier = (gameVariables.Civilizations[civilizationIndex].Happiness-70);

        tempModifier += happinessModifier;

    } else if (gameVariables.Civilizations[civilizationIndex].Happiness <= 25) {

        tempModifier -= 25;

    }

    if (sharedMethods::civilizationHasTechnology(civilizationIndex, "Masonry", gameVariables)) {

        tempModifier += 5;

    }

    gameVariables.Cities[cityIndex].ProductionModifier = (tempModifier/100);

}

void GameUpdater::updateCityUnitProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    gameVariables.Cities[cityIndex].Production += (gameVariables.Cities[cityIndex].ProductionFromTiles + gameVariables.Cities[cityIndex].ProductionPerTurn)*gameVariables.Cities[cityIndex].ProductionModifier;

    if (gameVariables.Cities[cityIndex].Production >= gameVariables.Cities[cityIndex].unitBeingProduced.productionCost && gameVariables.Cities[cityIndex].isProducing == true) {

        gameVariables.Cities[cityIndex].unitBeingProduced.position.x = gameVariables.Cities[cityIndex].position.x;
        gameVariables.Cities[cityIndex].unitBeingProduced.position.y = gameVariables.Cities[cityIndex].position.y;

        gameVariables.Cities[cityIndex].unitBeingProduced.parentCivilizationIndex = civilizationIndex;

        gameVariables.UnitsInGame.push_back(gameVariables.Cities[cityIndex].unitBeingProduced);

        gameVariables.Cities[cityIndex].Production -= gameVariables.Cities[cityIndex].unitBeingProduced.productionCost;

        gameVariables.Cities[cityIndex].isProducing = false;
        gameVariables.Cities[cityIndex].isProducingUnit = false;

        std::cout << gameVariables.Cities[cityIndex].cityName << " finished its production!" << std::endl;

    }

}

void GameUpdater::updateCityBuildingProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    gameVariables.Cities[cityIndex].Production += (gameVariables.Cities[cityIndex].ProductionFromTiles + gameVariables.Cities[cityIndex].ProductionPerTurn)*gameVariables.Cities[cityIndex].ProductionModifier;

    if (gameVariables.Cities[cityIndex].Production >= gameVariables.Cities[cityIndex].buildingBeingProduced.ProductionCost && gameVariables.Cities[cityIndex].isProducing == true) {

        gameVariables.Cities[cityIndex].FoodPerTurnFromCity += gameVariables.Cities[cityIndex].buildingBeingProduced.FoodYield;

        gameVariables.Cities[cityIndex].ProductionPerTurn += gameVariables.Cities[cityIndex].buildingBeingProduced.ProductionYield;

        gameVariables.Cities[cityIndex].GoldPerTurn += gameVariables.Cities[cityIndex].buildingBeingProduced.GoldYield;

        gameVariables.Cities[cityIndex].buildings.push_back (gameVariables.Cities[cityIndex].buildingBeingProduced.name);

        int indexToErase = -1;

        for (int i = 0; i < gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.size(); i++) {

            if (gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[i] == gameVariables.Cities[cityIndex].buildingBeingProduced.name) {

                indexToErase = i;

            }

        }

        if (indexToErase > -1 && indexToErase < gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.size()) {

            gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.erase (gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.begin() + indexToErase);

        }
        gameVariables.Cities[cityIndex].Production -= gameVariables.Cities[cityIndex].buildingBeingProduced.ProductionCost;

        gameVariables.Cities[cityIndex].isProducing = false;
        gameVariables.Cities[cityIndex].isProducingUnit = false;

        std::cout << gameVariables.Cities[cityIndex].cityName << " finished its production!" << std::endl;

    }

}
