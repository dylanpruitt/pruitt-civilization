#include <iostream>
#include <math.h>
#include <vector>
#include "AI.h"
#include "sharedMethods.h"

AI::AI()
{
    //ctor
}

AI::~AI()
{
    //dtor
}

enum directions : int {
    NORTH,
    WEST,
    SOUTH,
    EAST,
};

int AI::returnUnexploredTiles (int x, int y, int g, int range, GameVariables &game_variables) {

    int unexploredTiles = 0;

    for (int i = 0; i < worldMap.worldSize; i++) {

        for (int j = 0; j < worldMap.worldSize*4; j++) {

            if (sharedMethods::getDistance(x,y,i,j) < range && game_variables.Civilizations[g].WorldExplorationMap[i][j] == 0) {

                unexploredTiles++;

            }

        }

    }

    return unexploredTiles;

}

int AI::checkForUnexploredTerritory (int unitIndex, int g, GameVariables &game_variables) { /*flag (r: not obvious that int returns a direction*/

    int searchStartX = game_variables.UnitsInGame[unitIndex].position.x;
    int searchStartY = game_variables.UnitsInGame[unitIndex].position.y;

    int Direction = -1;

    int HighestNumberOfUnexploredTiles = 0;

    if (searchStartX > 0 && (worldMap.featureMap[searchStartX-1][searchStartY] != worldMap.mapTiles::OCEAN
    && ((worldMap.featureMap[searchStartX-1][searchStartY] != worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || game_variables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(g, searchStartX-1, searchStartY, worldMap)) {

        if (game_variables.Civilizations[g].WorldExplorationMap[searchStartX-1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX-1, searchStartY, g, 4, game_variables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::NORTH;

            }

        }
    }

    if (searchStartY > 0 && (worldMap.featureMap[searchStartX][searchStartY-1] != worldMap.mapTiles::OCEAN
    && ((worldMap.featureMap[searchStartX][searchStartY-1] != worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || game_variables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(g, searchStartX, searchStartY-1, worldMap)) {

        if (game_variables.Civilizations[g].WorldExplorationMap[searchStartX][searchStartY-1] == 0) {

            int ut = returnUnexploredTiles (searchStartX, searchStartY-1, g, 4, game_variables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::WEST;

            }

        }
    }

    if (searchStartX < worldMap.worldSize && (worldMap.featureMap[searchStartX+1][searchStartY] != worldMap.mapTiles::OCEAN
    && ((worldMap.featureMap[searchStartX+1][searchStartY] != worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || game_variables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(g, searchStartX+1, searchStartY, worldMap)) {

        if (game_variables.Civilizations[g].WorldExplorationMap[searchStartX+1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX+1, searchStartY, g, 4, game_variables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::SOUTH;

            }

        }
    }

    if (searchStartY < worldMap.worldSize*4 && (worldMap.featureMap[searchStartX][searchStartY+1] != worldMap.mapTiles::OCEAN
    && ((worldMap.featureMap[searchStartX][searchStartY+1] != worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)
    || game_variables.UnitsInGame[unitIndex].canCoastalEmbark == true))
    && sharedMethods::unitIsNotTrespassing(g, searchStartX, searchStartY+1, worldMap)) {

        if (game_variables.Civilizations[g].WorldExplorationMap[searchStartX][searchStartY+1] == 0) {

            int ut = returnUnexploredTiles (searchStartX, searchStartY+1, g, 4, game_variables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::EAST;

            }

        }
    }

    if (Direction == -1) {
        Direction = rand() % (directions::EAST - directions::NORTH) + directions::NORTH;

        if (Direction == directions::NORTH && searchStartX > 0 && (worldMap.featureMap[searchStartX-1][searchStartY] == worldMap.mapTiles::OCEAN || ((worldMap.featureMap[searchStartX][searchStartY-1] == worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::WEST && searchStartY > 0 && (worldMap.featureMap[searchStartX-1][searchStartY-1] == worldMap.mapTiles::OCEAN || ((worldMap.featureMap[searchStartX][searchStartY-1] == worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::SOUTH && searchStartX < worldMap.worldSize && (worldMap.featureMap[searchStartX+1][searchStartY] == worldMap.mapTiles::OCEAN || ((worldMap.featureMap[searchStartX+1][searchStartY] == worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

        if (Direction == directions::EAST && searchStartY < worldMap.worldSize*4 && (worldMap.featureMap[searchStartX][searchStartY+1] == worldMap.mapTiles::OCEAN || ((worldMap.featureMap[searchStartX][searchStartY+1] == worldMap.mapTiles::COAST && game_variables.UnitsInGame[unitIndex].canCoastalEmbark == false)))) {

            Direction = -1;
        }

    }

    return Direction;
}

void AI::moveUnit (int unitIndex, int g, GameVariables &game_variables) {

    int directionToMoveIn = checkForUnexploredTerritory (unitIndex, g, game_variables);

    switch (directionToMoveIn) {

        case directions::NORTH:
            sharedMethods::moveUnit (game_variables.UnitsInGame[unitIndex], game_variables.UnitsInGame[unitIndex].position.x-1, game_variables.UnitsInGame[unitIndex].position.y, game_variables.Civilizations[g], worldMap);
        break;
        case directions::WEST:
            sharedMethods::moveUnit (game_variables.UnitsInGame[unitIndex], game_variables.UnitsInGame[unitIndex].position.x, game_variables.UnitsInGame[unitIndex].position.y-1, game_variables.Civilizations[g], worldMap);
        break;
        case directions::SOUTH:
            sharedMethods::moveUnit (game_variables.UnitsInGame[unitIndex], game_variables.UnitsInGame[unitIndex].position.x+1, game_variables.UnitsInGame[unitIndex].position.y, game_variables.Civilizations[g], worldMap);
        break;
        case directions::EAST:
            sharedMethods::moveUnit (game_variables.UnitsInGame[unitIndex], game_variables.UnitsInGame[unitIndex].position.x, game_variables.UnitsInGame[unitIndex].position.y+1, game_variables.Civilizations[g], worldMap);
        break;
        case -1:
            game_variables.UnitsInGame[unitIndex].movementPoints = 0;
        break;

    }

}

void AI::moveAllUnitsBelongingToCiv (int g, GameVariables &game_variables) {

    for (unsigned int i = 0; i < game_variables.UnitsInGame.size(); i++) {

        if (game_variables.UnitsInGame[i].parentCivilizationIndex == g) {

            while (game_variables.UnitsInGame[i].movementPoints > 0) {

                moveUnit (i, g, game_variables);

            }

        }

    }

}

int AI::calculateResearchPriority (Research tech, int g, GameVariables &game_variables) {

    int Potential = 0; const int OVERALL_SCALE = 8;

    Potential += (tech.aiFocus_defense * game_variables.Civilizations[g].aiFocus_defense);
    Potential += (tech.aiFocus_diplomatic * game_variables.Civilizations[g].aiFocus_diplomatic);
    Potential += (tech.aiFocus_economic * game_variables.Civilizations[g].aiFocus_economic);
    Potential += (tech.aiFocus_exploration * game_variables.Civilizations[g].aiFocus_exploration);
    Potential += (tech.aiFocus_offense * game_variables.Civilizations[g].aiFocus_offense);
    Potential += (tech.aiFocus_overall_importance * OVERALL_SCALE);
    Potential += (tech.aiFocus_population * game_variables.Civilizations[g].aiFocus_population);
    Potential += (tech.aiFocus_production * game_variables.Civilizations[g].aiFocus_production);
    Potential += (tech.aiFocus_religion * game_variables.Civilizations[g].aiFocus_religion);
    Potential += (tech.aiFocus_scientific * game_variables.Civilizations[g].aiFocus_scientific);

    for (unsigned int i = 0; i < game_variables.Civilizations[g].learnedTechnologies.size(); i++) {

        if (game_variables.Civilizations[g].learnedTechnologies[i] == tech.researchName) {

            return 0;

        }

    }

    return Potential;

}

void AI::setTechToResearch (int g, int techIndex, GameVariables &game_variables) {

    if (techIndex > -1 && techIndex < game_variables.Civilizations[g].technologiesToResearch.size()) {

        game_variables.Civilizations[g].technologyBeingResearched = game_variables.Civilizations[g].technologiesToResearch[techIndex];

        std::cout << game_variables.Civilizations[g].CivName << " has begun researching " << game_variables.Civilizations[g].technologyBeingResearched.researchName << "!" << std::endl;
    }

}

void AI::setFocuses (Civilization &civ, GameVariables &game_variables) {

    /// The AI focuses are set to be any value from 2-8 (or 3-8 in the case of population growth).
    civ.aiFocus_defense = rand () % 6 + 2;
    civ.aiFocus_diplomatic = rand () % 6 + 2;
    civ.aiFocus_economic = rand () % 6 + 2;
    civ.aiFocus_exploration = rand () % 6 + 2;
    civ.aiFocus_offense = rand () % 6 + 2;
    civ.aiFocus_population = rand () % 5 + 3;
    civ.aiFocus_production = rand () % 6 + 2;
    civ.aiFocus_religion = rand () % 6 + 2;
    civ.aiFocus_scientific = rand () % 6 + 2;

}

void AI::setResearchPriority (int g, GameVariables &game_variables) {

    int HighestPriority = 0; int HighestPriorityIndex = -1;

    for (int i = 0; i < game_variables.Civilizations[g].technologiesToResearch.size(); i++) {

        if (calculateResearchPriority(game_variables.Civilizations[g].technologiesToResearch[i], g, game_variables) > HighestPriority && sharedMethods::CivilizationHasPrerequisiteTechs(g, game_variables.Civilizations[g].technologiesToResearch[i].researchName, game_variables) == true) {

            HighestPriority = calculateResearchPriority(game_variables.Civilizations[g].technologiesToResearch[i], g, game_variables);

            HighestPriorityIndex = i;

        }

    }

    if (HighestPriorityIndex >= 0) { setTechToResearch (g, HighestPriorityIndex, game_variables); }

}

int AI::returnUnitPotential (int g, Unit unit, GameVariables &game_variables) {

    int potential = 0; const int OVERALL_SCALE = 6;

    potential += (unit.aiFocus_defense * game_variables.Civilizations[g].aiFocus_defense);
    potential += (unit.aiFocus_economic * game_variables.Civilizations[g].aiFocus_economic);
    potential += (unit.aiFocus_exploration * game_variables.Civilizations[g].aiFocus_exploration);
    potential += (unit.aiFocus_offense * game_variables.Civilizations[g].aiFocus_offense);
    potential += (unit.aiFocus_overall_importance * OVERALL_SCALE);

    return potential;

}

void AI::produceUnit (int g, int cityIndex, std::string unitName, GameVariables &game_variables) {


    game_variables.Cities[cityIndex].isProducing = true;
    game_variables.Cities[cityIndex].isProducingUnit = true;
    game_variables.Cities[cityIndex].unitBeingProduced = game_variables.Units[sharedMethods::getUnitIndexByName(unitName, game_variables)];

    std::cout << unitName << " is being produced by " << game_variables.Civilizations[g].CivName << "!" << std::endl;

}

int AI::returnBuildingPotential (int g, Building building, GameVariables &game_variables) {

    int potential; const int OVERALL_SCALE = 8;

    potential += (building.aiFocus_defense * game_variables.Civilizations[g].aiFocus_defense);
    potential += (building.aiFocus_diplomatic * game_variables.Civilizations[g].aiFocus_diplomatic);
    potential += (building.aiFocus_economic * game_variables.Civilizations[g].aiFocus_economic);
    potential += (building.aiFocus_exploration * game_variables.Civilizations[g].aiFocus_exploration);
    potential += (building.aiFocus_offense * game_variables.Civilizations[g].aiFocus_offense);
    potential += (building.aiFocus_overall_importance * OVERALL_SCALE);
    potential += (building.aiFocus_population * game_variables.Civilizations[g].aiFocus_population);
    potential += (building.aiFocus_production * game_variables.Civilizations[g].aiFocus_production);
    potential += (building.aiFocus_religion * game_variables.Civilizations[g].aiFocus_religion);
    potential += (building.aiFocus_scientific * game_variables.Civilizations[g].aiFocus_scientific);

    return potential;

}

void AI::produceBuilding (int g, int cityIndex, std::string buildingName, GameVariables &game_variables) {

    game_variables.Cities[cityIndex].isProducing = true;
    game_variables.Cities[cityIndex].isProducingUnit = false;
    game_variables.Cities[cityIndex].buildingBeingProduced = game_variables.Buildings[sharedMethods::getBuildingIndexByName(buildingName, game_variables)];

    std::cout << buildingName << " is being produced by " << game_variables.Civilizations[g].CivName << "!" << std::endl;

}

void AI::produce (int g, int cityIndex, GameVariables &game_variables) {

    int HighestUnitPotential = 0; std::string HighestPotentialUnitName = "";

    for (unsigned int i = 0; i < game_variables.Civilizations[g].AvailableUnitsToCreate.size(); i++) {

        Unit unit = game_variables.Units[sharedMethods::getUnitIndexByName(game_variables.Civilizations[g].AvailableUnitsToCreate[i], game_variables)];

        int potential = returnUnitPotential (g, unit, game_variables);

        if (potential > HighestUnitPotential) {

            HighestPotentialUnitName = game_variables.Civilizations[g].AvailableUnitsToCreate[i];
            HighestUnitPotential = potential;

        }

    }

    int HighestBuildingPotential = 0;

    std::string HighestPotentialBuildingName = "";



    for (unsigned int i = 0; i < game_variables.Cities[cityIndex].AvailableBuildingsToCreate.size(); i++) {

        Building building = game_variables.Buildings[sharedMethods::getBuildingIndexByName(game_variables.Cities[cityIndex].AvailableBuildingsToCreate[i], game_variables)];

        int potential = returnBuildingPotential (g, building, game_variables);

        if (potential > HighestBuildingPotential) {

            HighestPotentialBuildingName = game_variables.Cities[cityIndex].AvailableBuildingsToCreate[i];
            HighestBuildingPotential = potential;

        }

    }

    if (HighestBuildingPotential >= HighestUnitPotential) {

        produceBuilding (g, cityIndex, HighestPotentialBuildingName, game_variables);

    } else {

        produceUnit (g, cityIndex, HighestPotentialUnitName, game_variables);

    }

}

int AI::returnTradeValue (Trade trade) {

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

    int tradeValue = totalTraderItemValue - totalRecipientItemValue;

    return tradeValue;

}

bool AI::hasLowHappiness (int g, GameVariables &game_variables) {

    if (game_variables.Civilizations[g].Happiness <= 70) {

        return true;

    }

    return false;

}

void AI::tradingLogic (int g, GameVariables &game_variables, std::vector<Trade> &trades) {

    if (hasLowHappiness(g, game_variables)) {

        int recipientIndex = -1;

        for (unsigned int i = 0; i < game_variables.Civilizations.size(); i++) {

            if (game_variables.Civilizations[i].resources.size() > 0) {

                recipientIndex = i;

                i = game_variables.Civilizations.size();

            }

        }

        if (recipientIndex != -1) {

            Trade trade;

            trade.recipientIndex = recipientIndex; trade.traderIndex = g;

            Resource resourceToTrade = game_variables.Civilizations[recipientIndex].resources[0];

            trade.goldSumFromTrader = 35;
            trade.goldSumFromRecipient = 0;
            trade.resourcesFromRecipient.push_back(resourceToTrade);

            trades.push_back(trade);

        }

    }

}

void AI::think (int g, GameVariables &game_variables, std::vector<Trade> &trades) {

    if (game_variables.Civilizations[g].technologyBeingResearched.researchName == "") {

        setResearchPriority (g, game_variables);

    }

    tradingLogic (g, game_variables, trades);

    for (unsigned int a = 0; a < game_variables.Cities.size(); a++) {

        if (game_variables.Cities[a].parentIndex == g && game_variables.Cities[a].isProducing == false) {

            produce (g, a, game_variables);

        }

    }

}
