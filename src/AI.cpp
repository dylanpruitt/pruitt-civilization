#include <iostream>
#include <math.h>
#include <vector>
#include "AI.h"
#include "sharedMethods.h"
#include "AStar.h"

AI::AI()
{
    //ctor
}

AI::~AI()
{
    //dtor
}

enum directions : int {
    NORTH = 1,
    WEST = 2,
    SOUTH = 3,
    EAST = 4,
};

int AI::returnUnexploredTiles (int x, int y, int civilizationIndex, int range, GameVariables &gameVariables) {

    int unexploredTiles = 0;

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (sharedMethods::getDistance(x,y,i,j) < range && gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i][j] == 0) {

                unexploredTiles++;

            }

        }

    }

    return unexploredTiles;

}

int AI::checkForUnexploredTerritory (int unitIndex, int civilizationIndex, GameVariables &gameVariables) { /*flag (r: not obvious that int returns a direction*/

    int searchStartX = gameVariables.UnitsInGame[unitIndex].position.x;
    int searchStartY = gameVariables.UnitsInGame[unitIndex].position.y;

    int Direction = -1;

    int HighestNumberOfUnexploredTiles = 0;

    if (searchStartX > 0 && (gameVariables.worldMap.featureMap[searchStartX-1][searchStartY] != gameVariables.worldMap.mapTiles::OCEAN
    && ((gameVariables.worldMap.featureMap[searchStartX-1][searchStartY] != gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(civilizationIndex, searchStartX-1, searchStartY, gameVariables.worldMap)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX-1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX-1, searchStartY, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::NORTH;

            }

        }
    }

    if (searchStartY > 0 && (gameVariables.worldMap.featureMap[searchStartX][searchStartY-1] != gameVariables.worldMap.mapTiles::OCEAN
    && ((gameVariables.worldMap.featureMap[searchStartX][searchStartY-1] != gameVariables.worldMap.mapTiles::COAST
    && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(civilizationIndex, searchStartX, searchStartY-1, gameVariables.worldMap)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX][searchStartY-1] == 0) {

            int ut = returnUnexploredTiles (searchStartX, searchStartY-1, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::WEST;

            }

        }
    }

    if (searchStartX < gameVariables.worldMap.worldSize && (gameVariables.worldMap.featureMap[searchStartX+1][searchStartY] != gameVariables.worldMap.mapTiles::OCEAN
    && ((gameVariables.worldMap.featureMap[searchStartX+1][searchStartY] != gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(civilizationIndex, searchStartX+1, searchStartY, gameVariables.worldMap)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX+1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX+1, searchStartY, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::SOUTH;

            }

        }
    }

    if (searchStartY < gameVariables.worldMap.worldSize*4 && (gameVariables.worldMap.featureMap[searchStartX][searchStartY+1] != gameVariables.worldMap.mapTiles::OCEAN
    && ((gameVariables.worldMap.featureMap[searchStartX][searchStartY+1] != gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(civilizationIndex, searchStartX, searchStartY+1, gameVariables.worldMap)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX][searchStartY+1] == 0) {

            int ut = returnUnexploredTiles (searchStartX, searchStartY+1, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::EAST;

            }

        }
    }

    if (Direction == -1) {
        Direction = rand() % (directions::EAST - directions::NORTH) + directions::NORTH;

        if (Direction == directions::NORTH && searchStartX > 0 && (gameVariables.worldMap.featureMap[searchStartX-1][searchStartY] == gameVariables.worldMap.mapTiles::OCEAN || ((gameVariables.worldMap.featureMap[searchStartX][searchStartY-1] == gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::WEST && searchStartY > 0 && (gameVariables.worldMap.featureMap[searchStartX-1][searchStartY-1] == gameVariables.worldMap.mapTiles::OCEAN || ((gameVariables.worldMap.featureMap[searchStartX][searchStartY-1] == gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::SOUTH && searchStartX < gameVariables.worldMap.worldSize && (gameVariables.worldMap.featureMap[searchStartX+1][searchStartY] == gameVariables.worldMap.mapTiles::OCEAN || ((gameVariables.worldMap.featureMap[searchStartX+1][searchStartY] == gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::EAST && searchStartY < gameVariables.worldMap.worldSize*4 && (gameVariables.worldMap.featureMap[searchStartX][searchStartY+1] == gameVariables.worldMap.mapTiles::OCEAN || ((gameVariables.worldMap.featureMap[searchStartX][searchStartY+1] == gameVariables.worldMap.mapTiles::COAST && gameVariables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

    }

    return Direction;
}

void AI::mapUnitPathToRuin (int civilizationIndex, GameVariables &gameVariables, int unitIndex) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::RUINS
            && gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i][j] == 1) {

                position RuinPosition = std::make_pair (i, j);

                position Source = std::make_pair (gameVariables.UnitsInGame[unitIndex].position.x,
                gameVariables.UnitsInGame[unitIndex].position.x);

                std::cout << gameVariables.UnitsInGame[unitIndex].position.x << ", " << gameVariables.UnitsInGame[unitIndex].position.y << " <-> " << i << ", " << j << std::endl;

                AStar::aStarSearch (gameVariables.worldMap.featureMap, Source, RuinPosition, gameVariables.UnitsInGame[unitIndex]);

                gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned = true;

                std::cout << civilizationIndex << " < destination found" << std::endl;

                return;

            }

        }

    }

}

void AI::moveUnit (int unitIndex, int civilizationIndex, GameVariables &gameVariables) {

    int directionToMoveIn = -1;

    if (gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned == false) {

        mapUnitPathToRuin (civilizationIndex, gameVariables, unitIndex);

    }

    if (gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.size() == 0) {

        gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned = false;

        directionToMoveIn = checkForUnexploredTerritory (unitIndex, civilizationIndex, gameVariables);

    } else {

        directionToMoveIn = gameVariables.UnitsInGame[unitIndex].moveDirectionQueue[0];

        gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.erase (gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.begin());

    }

    switch (directionToMoveIn) {

        case directions::NORTH:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x-1, gameVariables.UnitsInGame[unitIndex].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap);
        break;
        case directions::WEST:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x, gameVariables.UnitsInGame[unitIndex].position.y-1, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap);
        break;
        case directions::SOUTH:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x+1, gameVariables.UnitsInGame[unitIndex].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap);
        break;
        case directions::EAST:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x, gameVariables.UnitsInGame[unitIndex].position.y+1, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap);
        break;
        case -1:
            gameVariables.UnitsInGame[unitIndex].movementPoints = 0;
        break;

    }

    if (sharedMethods::UnitisOnAnAncientRuin(gameVariables.UnitsInGame[unitIndex], gameVariables.worldMap)) {

        std::cout << gameVariables.Civilizations[civilizationIndex].CivName << " Found a ruin!" << std::endl;

    }

}

void AI::moveAllUnitsBelongingToCiv (int civilizationIndex, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

            while (gameVariables.UnitsInGame[i].movementPoints > 0) {

                moveUnit (i, civilizationIndex, gameVariables);

            }

        }

    }

}

int AI::calculateResearchPriority (Research tech, int civilizationIndex, GameVariables &gameVariables) {

    int Potential = 0; const int OVERALL_SCALE = 8;

    Potential += (tech.aiFocus_defense * gameVariables.Civilizations[civilizationIndex].aiFocus_defense);
    Potential += (tech.aiFocus_diplomatic * gameVariables.Civilizations[civilizationIndex].aiFocus_diplomatic);
    Potential += (tech.aiFocus_economic * gameVariables.Civilizations[civilizationIndex].aiFocus_economic);
    Potential += (tech.aiFocus_exploration * gameVariables.Civilizations[civilizationIndex].aiFocus_exploration);
    Potential += (tech.aiFocus_offense * gameVariables.Civilizations[civilizationIndex].aiFocus_offense);
    Potential += (tech.aiFocus_overall_importance * OVERALL_SCALE);
    Potential += (tech.aiFocus_population * gameVariables.Civilizations[civilizationIndex].aiFocus_population);
    Potential += (tech.aiFocus_production * gameVariables.Civilizations[civilizationIndex].aiFocus_production);
    Potential += (tech.aiFocus_religion * gameVariables.Civilizations[civilizationIndex].aiFocus_religion);
    Potential += (tech.aiFocus_scientific * gameVariables.Civilizations[civilizationIndex].aiFocus_scientific);

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].learnedTechnologies.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].learnedTechnologies[i] == tech.researchName) {

            return 0;

        }

    }

    return Potential;

}

void AI::setTechToResearch (int civilizationIndex, int techIndex, GameVariables &gameVariables) {

    if (techIndex > -1 && techIndex < gameVariables.Civilizations[civilizationIndex].technologiesToResearch.size()) {

        gameVariables.Civilizations[civilizationIndex].technologyBeingResearched = gameVariables.Civilizations[civilizationIndex].technologiesToResearch[techIndex];

        std::cout << gameVariables.Civilizations[civilizationIndex].CivName << " has begun researching " << gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName << "!" << std::endl;
    }

}

void AI::setResearchPriority (int civilizationIndex, GameVariables &gameVariables) {

    int HighestPriority = 0; int HighestPriorityIndex = -1;

    for (int i = 0; i < gameVariables.Civilizations[civilizationIndex].technologiesToResearch.size(); i++) {

        if (calculateResearchPriority(gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i], civilizationIndex, gameVariables) > HighestPriority
        && sharedMethods::CivilizationHasPrerequisiteTechs(civilizationIndex, gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i].researchName,
        gameVariables) == true) {

            HighestPriority = calculateResearchPriority(gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i], civilizationIndex, gameVariables);

            HighestPriorityIndex = i;

        }

    }

    if (HighestPriorityIndex >= 0) { setTechToResearch (civilizationIndex, HighestPriorityIndex, gameVariables); }

}

int AI::returnUnitPotential (int civilizationIndex, Unit unit, GameVariables &gameVariables) {

    int potential = 0; const int OVERALL_SCALE = 6;

    potential += (unit.aiFocus_defense * gameVariables.Civilizations[civilizationIndex].aiFocus_defense);
    potential += (unit.aiFocus_economic * gameVariables.Civilizations[civilizationIndex].aiFocus_economic);
    potential += (unit.aiFocus_exploration * gameVariables.Civilizations[civilizationIndex].aiFocus_exploration);
    potential += (unit.aiFocus_offense * gameVariables.Civilizations[civilizationIndex].aiFocus_offense);
    potential += (unit.aiFocus_overall_importance * OVERALL_SCALE);

    return potential;

}

void AI::produceUnit (int civilizationIndex, int cityIndex, std::string unitName, GameVariables &gameVariables) {


    gameVariables.Cities[cityIndex].isProducing = true;
    gameVariables.Cities[cityIndex].isProducingUnit = true;
    gameVariables.Cities[cityIndex].unitBeingProduced = gameVariables.Units[sharedMethods::getUnitIndexByName(unitName, gameVariables)];

    std::cout << unitName << " is being produced by " << gameVariables.Civilizations[civilizationIndex].CivName << "!" << std::endl;

}

int AI::returnBuildingPotential (int civilizationIndex, Building building, GameVariables &gameVariables) {

    int potential; const int OVERALL_SCALE = 8;

    potential += (building.aiFocus_defense * gameVariables.Civilizations[civilizationIndex].aiFocus_defense);
    potential += (building.aiFocus_diplomatic * gameVariables.Civilizations[civilizationIndex].aiFocus_diplomatic);
    potential += (building.aiFocus_economic * gameVariables.Civilizations[civilizationIndex].aiFocus_economic);
    potential += (building.aiFocus_exploration * gameVariables.Civilizations[civilizationIndex].aiFocus_exploration);
    potential += (building.aiFocus_offense * gameVariables.Civilizations[civilizationIndex].aiFocus_offense);
    potential += (building.aiFocus_overall_importance * OVERALL_SCALE);
    potential += (building.aiFocus_population * gameVariables.Civilizations[civilizationIndex].aiFocus_population);
    potential += (building.aiFocus_production * gameVariables.Civilizations[civilizationIndex].aiFocus_production);
    potential += (building.aiFocus_religion * gameVariables.Civilizations[civilizationIndex].aiFocus_religion);
    potential += (building.aiFocus_scientific * gameVariables.Civilizations[civilizationIndex].aiFocus_scientific);

    return potential;

}

void AI::produceBuilding (int civilizationIndex, int cityIndex, std::string buildingName, GameVariables &gameVariables) {

    gameVariables.Cities[cityIndex].isProducing = true;
    gameVariables.Cities[cityIndex].isProducingUnit = false;
    gameVariables.Cities[cityIndex].buildingBeingProduced = gameVariables.Buildings[sharedMethods::getBuildingIndexByName(buildingName, gameVariables)];

    std::cout << buildingName << " is being produced by " << gameVariables.Civilizations[civilizationIndex].CivName << "!" << std::endl;

}

void AI::produce (int civilizationIndex, int cityIndex, GameVariables &gameVariables) {

    int HighestUnitPotential = 0; std::string HighestPotentialUnitName = "";

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

        Unit unit = gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i], gameVariables)];

        int potential = returnUnitPotential (civilizationIndex, unit, gameVariables);

        if (potential > HighestUnitPotential) {

            HighestPotentialUnitName = gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i];
            HighestUnitPotential = potential;

        }

    }

    int HighestBuildingPotential = 0;

    std::string HighestPotentialBuildingName = "";



    for (unsigned int i = 0; i < gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.size(); i++) {

        Building building = gameVariables.Buildings[sharedMethods::getBuildingIndexByName(gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[i], gameVariables)];

        int potential = returnBuildingPotential (civilizationIndex, building, gameVariables);

        if (potential > HighestBuildingPotential) {

            HighestPotentialBuildingName = gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[i];
            HighestBuildingPotential = potential;

        }

    }

    if (HighestBuildingPotential >= HighestUnitPotential) {

        produceBuilding (civilizationIndex, cityIndex, HighestPotentialBuildingName, gameVariables);

    } else {

        produceUnit (civilizationIndex, cityIndex, HighestPotentialUnitName, gameVariables);

    }

}

int AI::returnTradeValue (Trade trade, GameVariables &gameVariables) {

    int totalTraderItemValue = 0, TraderGoldValue = 0, TraderResourceValue = 0;

    int totalRecipientItemValue = 0, RecipientGoldValue = 0, RecipientResourceValue = 0;

    TraderGoldValue = trade.goldSumFromTrader; TraderGoldValue += trade.GPTFromTrader*25;

    for (unsigned int i = 0; i < trade.resourcesFromTrader.size(); i++) {

        TraderResourceValue += 35;

    }

    totalTraderItemValue = TraderGoldValue + TraderResourceValue;

    RecipientGoldValue = trade.goldSumFromRecipient; RecipientGoldValue += trade.GPTFromRecipient*25;

    for (unsigned int i = 0; i < trade.resourcesFromRecipient.size(); i++) {

        TraderResourceValue += 35;

    }

    totalRecipientItemValue = RecipientGoldValue + RecipientResourceValue;

    int tradeValue = totalTraderItemValue - (totalRecipientItemValue*((10-gameVariables.Civilizations[trade.recipientIndex].ai_fairness) / 5));

    return tradeValue;

}

bool AI::hasLowHappiness (int civilizationIndex, GameVariables &gameVariables) {

    if (gameVariables.Civilizations[civilizationIndex].Happiness <= 70) {

        return true;

    }

    return false;

}

void AI::tradingLogic (int civilizationIndex, GameVariables &gameVariables, std::vector<Trade> &trades) {

    if (hasLowHappiness(civilizationIndex, gameVariables)) {

        int recipientIndex = -1;

        for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

            if (gameVariables.Civilizations[i].resources.size() > 0) {

                recipientIndex = i;

                i = gameVariables.Civilizations.size();

            }

        }

        if (recipientIndex != -1) {

            Trade trade;

            trade.recipientIndex = recipientIndex; trade.traderIndex = civilizationIndex;

            Resource resourceToTrade = gameVariables.Civilizations[recipientIndex].resources[0];

            trade.goldSumFromTrader = 35*(gameVariables.Civilizations[civilizationIndex].ai_fairness/5);
            trade.goldSumFromRecipient = 0;
            trade.resourcesFromRecipient.push_back(resourceToTrade);

            trades.push_back(trade);

        }

    }

}

void AI::think (int civilizationIndex, GameVariables &gameVariables, std::vector<Trade> &trades) {

    if (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName == "") {

        setResearchPriority (civilizationIndex, gameVariables);

    }

    tradingLogic (civilizationIndex, gameVariables, trades);

    for (unsigned int a = 0; a < gameVariables.Cities.size(); a++) {

        if (gameVariables.Cities[a].parentIndex == civilizationIndex && gameVariables.Cities[a].isProducing == false) {

            produce (civilizationIndex, a, gameVariables);

        }

    }

}
