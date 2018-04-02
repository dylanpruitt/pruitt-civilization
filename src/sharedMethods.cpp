#include "sharedMethods.h"
#include <math.h>
#include <iostream>
#include <thread>
#include <climits>
#include <random>


namespace sharedMethods {

int returnAcceptableIntegerInput () {

    int Input = 0;

    std::cin >> Input;

    while (!std::cin) {

        std::cout << "Bad value!";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> Input;

    }

    return Input;

}

int bindIntegerInputToRange (int minimumAcceptableValue, int maximumAcceptableValue, int defaultValue) {

    int integerInput = returnAcceptableIntegerInput();

    if (integerInput > maximumAcceptableValue || integerInput < minimumAcceptableValue) {

        integerInput = defaultValue;

    }

    return integerInput;

}

int getDistance(int x, int y, int x2, int y2) {

    int distance = sqrt(((x-x2) * (x-x2)) + ((y-y2) * (y-y2)));

    return distance;

}

bool isResearchComplete (int civilizationIndex, GameVariables &gameVariables) {

    if (gameVariables.Civilizations[civilizationIndex].researchPoints >=
        gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.scienceCostToLearnResearch) {

        return true;

    } else {

        return false;

    }

}

int getUnitIndexByName (std::string name, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Units.size(); i++) {

        if (gameVariables.Units[i].name == name) {

            return i;

        }

    }

    std::cout << "Couldn't find unit!";

    return 0;

}

bool unitIsNotTrespassing (int civilizationIndex, int xPositionToMoveTo, int yPositionToMoveTo, WorldMap worldMap) {

    if (worldMap.WorldTerritoryMap[xPositionToMoveTo][yPositionToMoveTo] == 0 ||
    worldMap.WorldTerritoryMap[xPositionToMoveTo][yPositionToMoveTo] == civilizationIndex+1) {

        return true;

    } else {

        return false;

    }

}

bool UnitisOnAnAncientRuin (Unit &unit, WorldMap worldMap) {

    const int RUINS_CODE = 5;

    if (worldMap.featureMap[unit.position.x][unit.position.y] == RUINS_CODE) {

        return true;

    } else {

        return false;

    }

}

void getAncientRuinBenefits (Unit &unit, Civilization &civ, WorldMap worldMap) {

    if (UnitisOnAnAncientRuin(unit, worldMap)) {

        civ.researchPoints = civ.technologyBeingResearched.scienceCostToLearnResearch;

        worldMap.featureMap[unit.position.x][unit.position.y] = worldMap.mapTiles::GRASSLAND;

    }

}

void moveUnit (Unit &unit, int xPositionToMoveTo, int yPositionToMoveTo, Civilization &civ, WorldMap worldMap) {

    bool isFlatTerrain = true;

    if
    (((unit.canCoastalEmbark == false && worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] != worldMap.mapTiles::COAST) || unit.canCoastalEmbark)
    && ((unit.canCrossOceans == false && worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] != worldMap.mapTiles::OCEAN) || unit.canCrossOceans)
    && unitIsNotTrespassing(unit.parentCivilizationIndex, xPositionToMoveTo, yPositionToMoveTo, worldMap)) {

        if (worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] == worldMap.mapTiles::MOUNTAIN
            || worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] == worldMap.mapTiles::FOREST) {

            isFlatTerrain = false;
        }

        if (isFlatTerrain) { unit.movementPoints--; } else { unit.movementPoints -= unit.terrainMoveCost; }

        unit.position.setCoordinates(xPositionToMoveTo, yPositionToMoveTo);

        getAncientRuinBenefits (unit, civ, worldMap);
    }

    for (int i = 0; i < worldMap.worldSize; i++) {

        for (int j = 0; j < worldMap.worldSize*4; j++) {

            if (getDistance(i,j,unit.position.x,unit.position.y) <= 2) {
                civ.WorldExplorationMap[i][j] = 1;
            }

        }
    }

}

int getBuildingIndexByName (std::string name, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Buildings.size(); i++) {

        if (gameVariables.Buildings[i].name == name) {

            return i;

        }

    }

    std::cout << "Couldn't find building!";

    return 0;

}

int getResearchIndexByName (int civilizationIndex, std::string techName, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].technologiesToResearch.size(); i++) {

        if (techName == gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i].researchName) {

            return i;

        }

    }

    return -1;

}

bool CivilizationHasPrerequisiteTechs (int civilizationIndex, std::string techName, GameVariables &gameVariables) {

    int techPrequisitesMet = 0;

    int researchIndex = getResearchIndexByName (civilizationIndex, techName, gameVariables);

    if (researchIndex > -1) {

        for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired.size(); i++) {

            for (unsigned int j = 0; j < gameVariables.Civilizations[civilizationIndex].learnedTechnologies.size(); j++) {

                if (gameVariables.Civilizations[civilizationIndex].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired[i] == gameVariables.Civilizations[civilizationIndex].learnedTechnologies[j]) {

                    techPrequisitesMet++;

                }

            }

        }

    }

    if (techPrequisitesMet == gameVariables.Civilizations[civilizationIndex].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired.size()) {
        return true;
    } else {
        return false;
    }

}

bool civilizationHasTechnology (int civilizationIndex, std::string techName, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].learnedTechnologies.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].learnedTechnologies[i] == techName) {

            return true;

        }

    }

    return false;

}

bool isTileBorderingCivilizationTerritory (int x, int y, int civilizationIndex, GameVariables gameVariables) {

    for (int i = -1; i < 2; i++) {

        for (int j = -1; j < 2; j++) {

            if (x+i >= 0 && x+i <= gameVariables.worldMap.worldSize && y+j >= 0 && y+j <= gameVariables.worldMap.worldSize*4) {

                if (gameVariables.worldMap.WorldTerritoryMap[i+x][j+y] == civilizationIndex+1 && sharedMethods::getDistance (x+i, y+j, x, y) <= 1) {

                    return true;

                }

            }

        }

    }

    return false;

}

int getTileFoodYield (int i, int j, GameVariables &gameVariables) {

    switch (gameVariables.worldMap.featureMap[i][j]) {

        case gameVariables.worldMap.mapTiles::OCEAN:
            return 1;
        break;

        case gameVariables.worldMap.mapTiles::COAST:
            return 1;
        break;

        case gameVariables.worldMap.mapTiles::GRASSLAND:
            return 2;
        break;

        case gameVariables.worldMap.mapTiles::FOREST:
            return 1;
        break;

        default:
            return 0;
        break;

    }

}

int getTileProductionYield (int i, int j, GameVariables &gameVariables) {

    switch (gameVariables.worldMap.featureMap[i][j]) {

        case gameVariables.worldMap.mapTiles::FOREST:
            return 1;
        break;

        case gameVariables.worldMap.mapTiles::MOUNTAIN:
            return 2;
        break;

        default:
            return 0;
        break;

    }

}

void foundCity (int x, int y, int civilizationIndex, GameVariables &gameVariables) {

    City city;

    city.position.x = x;
    city.position.y = y;

    int cityNameIndex = rand() % gameVariables.Civilizations[civilizationIndex].cityNames.size();

    city.cityName = gameVariables.Civilizations[civilizationIndex].cityNames[cityNameIndex];

    city.AvailableBuildingsToCreate.push_back("Granary");
    city.AvailableBuildingsToCreate.push_back("Library");
    city.AvailableBuildingsToCreate.push_back("Market");

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {
        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {
            if (getDistance(i,j,x,y) <= 1) {
                gameVariables.worldMap.WorldTerritoryMap[i][j] = civilizationIndex+1;
            }
        }
    }

    city.parentIndex = civilizationIndex;

    gameVariables.Cities.push_back(city);

    assignWorkByPopulation(gameVariables.Cities.size() - 1, false, gameVariables);

}

void assignWorkByPopulation (int cityIndex, bool stopAfterNeededAmountIsCollected, GameVariables &gameVariables) {

    int assignedCitizens = 0;

    int highestFoodValueTiles[gameVariables.Cities[cityIndex].Population];

    int tileArraySize = (sizeof(highestFoodValueTiles)/sizeof(*highestFoodValueTiles));

    int bestFoodTileXPositions[gameVariables.Cities[cityIndex].Population];

    int bestFoodTileYPositions[gameVariables.Cities[cityIndex].Population];

    for (int a = 0; a < tileArraySize; a++) { highestFoodValueTiles[a] = 0; }

    gameVariables.Cities[cityIndex].FoodPerTurnFromTiles = 0;

    gameVariables.Cities[cityIndex].ProductionFromTiles = 0;

    for (int i = -4; i < 4; i++) {

        if (i + gameVariables.Cities[cityIndex].position.x < 0) {

            i++;

        }

        for (int j = -4; j < 4; j++) {

            if (j + gameVariables.Cities[cityIndex].position.y < 0) {

                j++;

            }

            if (i + gameVariables.Cities[cityIndex].position.x >= 0 && j + gameVariables.Cities[cityIndex].position.y >= 0) {

                int yield = getTileFoodYield (i + gameVariables.Cities[cityIndex].position.x, j + gameVariables.Cities[cityIndex].position.y, gameVariables);

                for (int k = 0; k < tileArraySize; k++) {

                    if (yield > highestFoodValueTiles[k] && gameVariables.worldMap.WorldTerritoryMap[i + gameVariables.Cities[cityIndex].position.x][j + gameVariables.Cities[cityIndex].position.y] == gameVariables.Cities[cityIndex].parentIndex+1) {

                        highestFoodValueTiles[k] = yield;
                        bestFoodTileXPositions[k] = i + gameVariables.Cities[cityIndex].position.x;
                        bestFoodTileYPositions[k] = j + gameVariables.Cities[cityIndex].position.y;
                        gameVariables.Cities[cityIndex].FoodPerTurnFromTiles += yield;
                        gameVariables.Cities[cityIndex].ProductionFromTiles += getTileProductionYield(i + gameVariables.Cities[cityIndex].position.x, j + gameVariables.Cities[cityIndex].position.y, gameVariables);
                        assignedCitizens++;

                        k = tileArraySize;

                        int foodIncrease = ((gameVariables.Cities[cityIndex].FoodPerTurnFromCity + gameVariables.Cities[cityIndex].FoodPerTurnFromTiles) - (gameVariables.Cities[cityIndex].Population*2));

                        if (foodIncrease > 0 && stopAfterNeededAmountIsCollected == true) {

                            i = 4;

                        }

                    }

                }

            }


        }

    }


}


}
