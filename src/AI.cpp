#include <iostream>
#include <math.h>
#include <vector>
#include "AI.h"
#include "sharedMethods.h"
#include "AStar.h"
#include "LoanEvent.h"
#include "Loan.h"

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

bool AI::positionIsOnMap (int x, int y, GameVariables &gameVariables) {

    if (x > 0 && y > 0 && x < gameVariables.worldMap.worldSize && y < gameVariables.worldMap.worldSize*4) {

        return true;

    } else {

        return false;

    }

}

bool AI::unitCanMoveOnTerrain (int x, int y, int unitIndex, GameVariables &gameVariables) {

    if (((gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::OCEAN
            && gameVariables.UnitsInGame[unitIndex].domain.canCrossOceans == false)
        || gameVariables.UnitsInGame[unitIndex].domain.canCrossOceans)

        && ((gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::COAST
            && gameVariables.UnitsInGame[unitIndex].domain.canCoastalEmbark == false)
        || gameVariables.UnitsInGame[unitIndex].domain.canCoastalEmbark)) {

        return true;

    } else {

        return false;

    }

}

bool AI::unitCanMoveToTile (int x, int y, int unitIndex, int civilizationIndex, GameVariables &gameVariables) {

    if (positionIsOnMap (x, y, gameVariables) && unitCanMoveOnTerrain (x, y, unitIndex, gameVariables)
    && sharedMethods::unitIsNotTrespassing(civilizationIndex, x, y, gameVariables.worldMap)) {

        return true;

    } else {

        return false;

    }
}

int AI::checkForUnexploredTerritory (int unitIndex, int civilizationIndex, GameVariables &gameVariables) { /*flag (r: not obvious that int returns a direction*/

    int searchStartX = gameVariables.UnitsInGame[unitIndex].position.x;
    int searchStartY = gameVariables.UnitsInGame[unitIndex].position.y;

    int Direction = -1;

    int HighestNumberOfUnexploredTiles = 0;

    if (unitCanMoveToTile (searchStartX-1, searchStartY, unitIndex, civilizationIndex, gameVariables)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX-1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX-1, searchStartY, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::NORTH;

            }

        }
    }

    if (unitCanMoveToTile (searchStartX, searchStartY-1, unitIndex, civilizationIndex, gameVariables)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX][searchStartY-1] == 0) {

            int ut = returnUnexploredTiles (searchStartX, searchStartY-1, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::WEST;

            }

        }
    }

    if (unitCanMoveToTile (searchStartX+1, searchStartY, unitIndex, civilizationIndex, gameVariables)) {

        if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[searchStartX+1][searchStartY] == 0) {

            int ut = returnUnexploredTiles (searchStartX+1, searchStartY, civilizationIndex, 4, gameVariables);

            if (ut > HighestNumberOfUnexploredTiles) {
                HighestNumberOfUnexploredTiles = ut;
                Direction = directions::SOUTH;

            }

        }
    }

    if (unitCanMoveToTile (searchStartX, searchStartY-1, unitIndex, civilizationIndex, gameVariables)) {

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

        if (Direction == directions::NORTH && !(unitCanMoveToTile (searchStartX-1, searchStartY, unitIndex, civilizationIndex, gameVariables))) {
            Direction = -1;
        }

        if (Direction == directions::WEST && !(unitCanMoveToTile (searchStartX, searchStartY-1, unitIndex, civilizationIndex, gameVariables))) {
            Direction = -1;
        }

        if (Direction == directions::SOUTH && !(unitCanMoveToTile (searchStartX+1, searchStartY, unitIndex, civilizationIndex, gameVariables))) {
            Direction = -1;
        }

        if (Direction == directions::EAST &&  !(unitCanMoveToTile (searchStartX, searchStartY+1, unitIndex, civilizationIndex, gameVariables))) {
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

                mapUnitPath (i, j, gameVariables, unitIndex);

            }

        }

    }

}

void AI::moveUnit (int unitIndex, int civilizationIndex, GameVariables &gameVariables) {

    if (gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned == false) {

        if (returnGroupNameFromIndex(civilizationIndex, gameVariables.UnitsInGame[unitIndex].parentGroupingIndex, gameVariables) == "special") {

            searchMapForSettlementLocation (civilizationIndex, unitIndex, gameVariables);

        } else if (returnGroupNameFromIndex(civilizationIndex, gameVariables.UnitsInGame[unitIndex].parentGroupingIndex, gameVariables) == "exploration") {

            mapUnitPathToRuin (civilizationIndex, gameVariables, unitIndex);

        }

    }

    int directionToMoveIn = -1;

    if (gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.size() == 0) {

        if (gameVariables.UnitsInGame[unitIndex].name == "Settler" && gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned == true) {

            sharedMethods::foundCity (gameVariables.UnitsInGame[unitIndex].position.x,
                gameVariables.UnitsInGame[unitIndex].position.y, civilizationIndex,
                gameVariables);

            gameVariables.UnitsInGame.erase (gameVariables.UnitsInGame.begin() + unitIndex);

        }

    } else {

        directionToMoveIn = gameVariables.UnitsInGame[unitIndex].moveDirectionQueue[0];

        gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.erase (gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.begin());

    }

    switch (directionToMoveIn) {

        case directions::NORTH:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x-1, gameVariables.UnitsInGame[unitIndex].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables);
        break;
        case directions::WEST:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x, gameVariables.UnitsInGame[unitIndex].position.y-1, gameVariables.Civilizations[civilizationIndex], gameVariables);
        break;
        case directions::SOUTH:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x+1, gameVariables.UnitsInGame[unitIndex].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables);
        break;
        case directions::EAST:
            sharedMethods::moveUnit (gameVariables.UnitsInGame[unitIndex], gameVariables.UnitsInGame[unitIndex].position.x, gameVariables.UnitsInGame[unitIndex].position.y+1, gameVariables.Civilizations[civilizationIndex], gameVariables);
        break;
        case -1:
            gameVariables.UnitsInGame[unitIndex].movementPoints = 0;
        break;

    }

    if (sharedMethods::UnitisOnAnAncientRuin(gameVariables.UnitsInGame[unitIndex], gameVariables.worldMap)) {

        std::cout << gameVariables.Civilizations[civilizationIndex].CivName << " found a ruin!" << std::endl;

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

    int potential = 0; const int OVERALL_SCALE = 8;

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

        if (gameVariables.Cities[cityIndex].lastProductionType != "unit" && civilizationIsBehindTargetAmountOfUnits (civilizationIndex, gameVariables)
            && unitProductionMode == "gradual") {

            potential += 100;

        }

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

    if (gameVariables.Civilizations[civilizationIndex].Happiness <= 40) {

        return true;

    }

    return false;

}

void AI::tradingLogic (int civilizationIndex, GameVariables &gameVariables) {

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

            gameVariables.trades.push_back(trade);

        }

    }

}

void AI::decideIfCivilizationShouldOfferLoan (int civilizationIndex, GameVariables &gameVariables) {

    int CivilizationToLoanTo = -1; int highestValue = 0; int temp_value = 0;

    for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

        if (i != civilizationIndex) {

            temp_value = calculatePotentialLoanValue (civilizationIndex, i, gameVariables);

            if (temp_value > highestValue) {

                highestValue = temp_value;

                if (temp_value >= 30) {

                    CivilizationToLoanTo = i;

                }

            }

        }

    }

    if (CivilizationToLoanTo != -1) {

        Loan temp_loan;

        temp_loan.creditorCivilizationIndex = civilizationIndex;
        temp_loan.debtorCivilizationIndex = CivilizationToLoanTo;

        temp_loan.amountDue = 75;

        gameVariables.activeLoans.push_back (temp_loan);

        sharedMethods::createNewLoanEventNotification (civilizationIndex, gameVariables, temp_loan);

    }

}

int AI::calculatePotentialLoanValue (int civilizationIndex, int potentialCivilizationIndex, GameVariables &gameVariables) {

    int loan_value = 0;

    loan_value += gameVariables.Civilizations[civilizationIndex].relationsWithOtherCivilizations[potentialCivilizationIndex];

    if (gameVariables.Civilizations[potentialCivilizationIndex].Gold >= 0 && gameVariables.Civilizations[potentialCivilizationIndex].Gold < 100) {

        loan_value += 50;

    }

    if (gameVariables.Civilizations[civilizationIndex].Gold <= 150) {

        loan_value -= 100;

    }

    return loan_value;

}



void AI::think (int civilizationIndex, GameVariables &gameVariables) {

    updateThreatLevel (civilizationIndex, gameVariables);

    updateTargetAmountOfUnits (civilizationIndex, gameVariables);

    groupUnits (civilizationIndex, gameVariables);

    if (returnGroupSizeFromName (civilizationIndex, "training", gameVariables) > 0) {

        int trainingGroupIndex = returnGroupIndexFromName (civilizationIndex, "training", gameVariables);

        for (unsigned int i = 0; i < gameVariables.Civilizations [civilizationIndex].unitGroups [trainingGroupIndex].memberUnitIndices.size (); i++) {

            updateUnitTraining (gameVariables.Civilizations [civilizationIndex].unitGroups [trainingGroupIndex].memberUnitIndices [i], gameVariables);

        }

    }

    if (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName == "") {

        setResearchPriority (civilizationIndex, gameVariables);

    }

    tradingLogic (civilizationIndex, gameVariables);

    decideIfCivilizationShouldOfferLoan (civilizationIndex, gameVariables);

    for (unsigned int a = 0; a < gameVariables.Cities.size(); a++) {

        if (gameVariables.Cities[a].parentIndex == civilizationIndex && gameVariables.Cities[a].isProducing == false) {

            produce (civilizationIndex, a, gameVariables);

        }

    }

}

void AI::searchMapForSettlementLocation (int civilizationIndex, int unitIndex, GameVariables &gameVariables) {

    int highestSearchValue = -1;

    int bestFindXPosition = -1, bestFindYPosition = -1;

    for (unsigned int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (unsigned int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            int searchValue = calculateTileSettlementValue (i, j, gameVariables);

            if (searchValue > highestSearchValue && gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i][j] == 1) {

                highestSearchValue = searchValue;

                bestFindXPosition = i;
                bestFindYPosition = j;

            }

        }

    }

    int NO_BEST_FIND = -1;

    if (bestFindXPosition != NO_BEST_FIND && bestFindYPosition != NO_BEST_FIND) {

        mapPathToCity (bestFindXPosition, bestFindYPosition, gameVariables, unitIndex);

    }

}

int AI::calculateTileSettlementValue (int x, int y, GameVariables &gameVariables) {

    int tileValue = 0;

    for (int i = -1; i < 2; i++) {

        for (int j = -1; j < 2; j++) {

            if (x+i >= 0 && y+j >= 0 && x+i <= gameVariables.worldMap.worldSize && y+j <= gameVariables.worldMap.worldSize*4) {

                if (gameVariables.worldMap.WorldResourceMap[x+i][y+j].ResourceCode != 0) {

                    tileValue += 15;

                }

                if (gameVariables.worldMap.featureMap[x+i][y+j] == gameVariables.worldMap.MOUNTAIN) {

                    tileValue += 5;

                }

                if (positionIsNotTooCloseToExistingCities (x+i, y+j, gameVariables) == false) {

                    tileValue = -1;

                }

            }

        }

    }

    return tileValue;

}

bool AI::positionIsNotTooCloseToExistingCities (int x, int y, GameVariables &gameVariables) {

    int distanceFromPositionToClosestCity = 10000;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        int tempDistance = sharedMethods::getDistance (x, y, gameVariables.Cities[i].position.x, gameVariables.Cities[i].position.y);

        if (tempDistance < distanceFromPositionToClosestCity) {

            distanceFromPositionToClosestCity = tempDistance;

        }

    }

    int CITY_DISTANCE_MINIMUM = 4;

    if (distanceFromPositionToClosestCity < CITY_DISTANCE_MINIMUM) {

        return false;

    } else {

        return true;

    }

}

void AI::mapPathToCity (int x, int y, GameVariables &gameVariables, int unitIndex) {

    mapUnitPath (x, y, gameVariables, unitIndex);

}

void AI::mapUnitPath (int x, int y, GameVariables &gameVariables, int unitIndex) {

    position destination = std::make_pair (x, y);

    position Source = std::make_pair (gameVariables.UnitsInGame[unitIndex].position.x,

    gameVariables.UnitsInGame[unitIndex].position.y);

    AStar::aStarSearch (gameVariables.worldMap.featureMap, Source, destination, gameVariables.UnitsInGame[unitIndex]);

    gameVariables.UnitsInGame[unitIndex].destinationHasBeenAssigned = true;

    return;


}

void AI::updateTargetAmountOfUnits (int civilizationIndex, GameVariables &gameVariables) {

    int standardAmount = sharedMethods::returnNumberOfCitiesCivilizationOwns (civilizationIndex, gameVariables) + 1, defenseAmount = 0;

    if (gameVariables.Civilizations [civilizationIndex].ai_threatened_level >= 35) {

        double targetSizeMultiplier = 1 + (gameVariables.Civilizations [civilizationIndex].aiFocus_defense / 10);

        defenseAmount = sharedMethods::returnNumberOfCitiesCivilizationOwns (civilizationIndex, gameVariables) * targetSizeMultiplier;

    }

    int totalAmount = standardAmount + defenseAmount;

    targetAmountOfUnits = totalAmount;

}

void AI::groupUnits (int civilizationIndex, GameVariables &gameVariables) {

    int numberOfUnitsNeededForGarrison = sharedMethods::returnNumberOfCitiesCivilizationOwns (civilizationIndex, gameVariables);

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

            if (gameVariables.UnitsInGame[i].name == "Settler") { assignUnitToGroup (civilizationIndex, i, "special", gameVariables); }

            if (gameVariables.UnitsInGame[i].name == "Explorer") { assignUnitToGroup (civilizationIndex, i, "exploration", gameVariables); }

            int garrisonSize = returnGroupSizeFromName (civilizationIndex, "garrison", gameVariables);

            if (garrisonSize < numberOfUnitsNeededForGarrison && unitIsNotAssignedToGroup (i, gameVariables)) {

                assignUnitToGroup (civilizationIndex, i, "garrison", gameVariables);

            } else if (unitIsNotAssignedToGroup (i, gameVariables)) {

                assignUnitToGroup (civilizationIndex, i, "training", gameVariables);

            }

        }

    }

}

void AI::assignUnitToGroup (int civilizationIndex, int unitIndex, std::string groupName, GameVariables &gameVariables) {

    if (unitIsNotAssignedToGroup (unitIndex, gameVariables)) {

        if (groupDoesNotExist (civilizationIndex, groupName, gameVariables)) {

            int rgb[3] = {128, 128, 128};

            gameVariables.Civilizations[civilizationIndex].addNewGrouping (groupName, rgb);

        }

        gameVariables.Civilizations[civilizationIndex].unitGroups[returnGroupIndexFromName(civilizationIndex, groupName, gameVariables)].memberUnitIndices.push_back (unitIndex);

        gameVariables.UnitsInGame[unitIndex].parentGroupingIndex = returnGroupIndexFromName (civilizationIndex, groupName, gameVariables);

        std::cout << gameVariables.Civilizations [civilizationIndex].CivName << " added " << gameVariables.UnitsInGame[unitIndex].name << " to " << groupName << std::endl;

    }

}

bool AI::unitIsNotAssignedToGroup (int unitIndex, GameVariables &gameVariables) {

    const int NOT_ASSIGNED_TO_GROUP = -1;
    if (gameVariables.UnitsInGame[unitIndex].parentGroupingIndex != NOT_ASSIGNED_TO_GROUP) {

        return false;

    }

    return true;

}

bool AI::groupDoesNotExist (int civilizationIndex, std::string groupName, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].unitGroups.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].unitGroups[i].name == groupName) {

            return false;
        }

    }

    return true;

}

int AI::returnGroupIndexFromName (int civilizationIndex, std::string groupName, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].unitGroups.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].unitGroups[i].name == groupName) {

            return i;
        }

    }

    return -1; /// Can't find the group

}

int AI::returnGroupSizeFromName (int civilizationIndex, std::string groupName, GameVariables &gameVariables) {

    if (groupDoesNotExist (civilizationIndex, groupName, gameVariables)) {

        return 0;

    } else {

        int groupIndex = returnGroupIndexFromName (civilizationIndex, groupName, gameVariables);

        return gameVariables.Civilizations [civilizationIndex].unitGroups [groupIndex].memberUnitIndices.size ();

    }

}

std::string AI::returnGroupNameFromIndex (int civilizationIndex, int groupIndex, GameVariables &gameVariables) {

    if (groupIndex != -1 && groupIndex < gameVariables.Civilizations[civilizationIndex].unitGroups.size()) {

        return gameVariables.Civilizations[civilizationIndex].unitGroups[groupIndex].name;

    } else {

        return "no_group";

    }

}

bool AI::civilizationIsBehindTargetAmountOfUnits (int civilizationIndex, GameVariables &gameVariables) {

    int amountOfUnitsOwned = 0;

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size (); i++) {

        if (gameVariables.UnitsInGame [i].parentCivilizationIndex == civilizationIndex) {

            amountOfUnitsOwned++;

        }

    }

    if (amountOfUnitsOwned < targetAmountOfUnits) {

        return true;

    } else {

        return false;

    }

}

void AI::updateUnitTraining (int unitIndex, GameVariables &gameVariables) {

    if (!gameVariables.UnitsInGame [unitIndex].destinationHasBeenAssigned) {

        assignUnitToTrain (unitIndex, gameVariables);

    }

    if (gameVariables.UnitsInGame[unitIndex].moveDirectionQueue.size() == 0 && !gameVariables.UnitsInGame[unitIndex].isTraining) {

        gameVariables.UnitsInGame [unitIndex].isTraining = true;

    }

}

void AI::assignUnitToTrain (int unitIndex, GameVariables &gameVariables) {

    int mostOccuringTileCode = returnMostOccuringTileCodeInCivilizationTerritory (gameVariables.UnitsInGame [unitIndex].parentCivilizationIndex,
                                                                                        gameVariables);

    Position destination = returnLocationOfNearestTileType (mostOccuringTileCode, unitIndex, gameVariables);

    mapUnitPath (destination.x, destination.y, gameVariables, unitIndex); std::cout << "unit path assigned!" << std::endl;

}

Position AI::returnLocationOfNearestTileType (int tileCode, int unitIndex, GameVariables &gameVariables) {

    int closestXCoordinate = 0, closestYCoordinate = 0; int closestDistance = 10000;

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.featureMap [i][j] == tileCode) {

                int distance = sharedMethods::getDistance (i, j, gameVariables.UnitsInGame [unitIndex].position.x,
                                                            gameVariables.UnitsInGame [unitIndex].position.y);

                if (distance < closestDistance) {

                    closestDistance = distance;

                    closestXCoordinate = i;
                    closestYCoordinate = j;

                }

            }

        }

    }

    Position position;

    position.x = closestXCoordinate;
    position.y = closestYCoordinate;

    return position;

}

int AI::returnMostOccuringTileCodeInCivilizationTerritory (int civilizationIndex, GameVariables &gameVariables) {

    int tileCodeOccurances [8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap [i][j] == civilizationIndex+1) {

                tileCodeOccurances [gameVariables.worldMap.featureMap [i][j]] ++;

            }

        }

    }

    int mostOccuringTileCode = 2, highestAmountOfOccurances = tileCodeOccurances [0];

    for (int i = 0; i < 8; i++) {

        if (tileCodeOccurances [i] > highestAmountOfOccurances && i >= 2) {

            highestAmountOfOccurances = tileCodeOccurances [i];

            mostOccuringTileCode = i;

        }

    }

    std::cout << mostOccuringTileCode << " occurs da most" << std::endl;

    return mostOccuringTileCode;

}

void AI::updateThreatLevel (int civilizationIndex, GameVariables &gameVariables) {

    int threatChange = -2; /// AI relaxes if there isn't anything still threatening it

    for (unsigned int i = 0; i < gameVariables.Civilizations.size (); i++) {

        if (civilizationIndex != i) {

            threatChange += 2 * returnNumberOfUnitsFromCivilizationBorderingTerritory (civilizationIndex, i, gameVariables);

        }

    }

    if (gameVariables.Civilizations [civilizationIndex].ai_threatened_level < 0) {

        gameVariables.Civilizations [civilizationIndex].ai_threatened_level = 0;

    }

    if (gameVariables.Civilizations [civilizationIndex].ai_threatened_level > 100) {

        gameVariables.Civilizations [civilizationIndex].ai_threatened_level = 100;

    }

    gameVariables.Civilizations [civilizationIndex].ai_threatened_level += threatChange;

    std::cout << "threat " << gameVariables.Civilizations [civilizationIndex].ai_threatened_level <<  " | " << threatChange << std::endl;

}

int AI::returnNumberOfUnitsFromCivilizationBorderingTerritory (int civilizationIndex, int unitOwnerIndex, GameVariables &gameVariables) {

    int unitCount = 0;

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size (); i++) {

        if (gameVariables.UnitsInGame [i].parentCivilizationIndex == unitOwnerIndex && unitIsBorderingTerritory (i, civilizationIndex, gameVariables)) {

            unitCount ++; std::cout << unitOwnerIndex << std::endl;

        }

    } std::cout << unitCount << " units bordering " << std::endl;

    return unitCount;

}

bool AI::unitIsBorderingTerritory (int unitIndex, int civilizationIndex, GameVariables &gameVariables) {

    int unitX = gameVariables.UnitsInGame [unitIndex].position.x, unitY = gameVariables.UnitsInGame [unitIndex].position.y;

    if (gameVariables.UnitsInGame [unitIndex].parentCivilizationIndex == civilizationIndex ) { return false; }

    if (unitX > 0) {

        if (gameVariables.worldMap.WorldTerritoryMap [unitX-1][unitY] == civilizationIndex+1) {

            return true;

        }

    }

    if (unitX < gameVariables.worldMap.worldSize-1) {

        if (gameVariables.worldMap.WorldTerritoryMap [unitX+1][unitY] == civilizationIndex+1) {

            return true;

        }

    }

    if (unitY > 0) {

        if (gameVariables.worldMap.WorldTerritoryMap [unitX][unitY-1] == civilizationIndex+1) {

            return true;

        }

    }

    if (unitY < gameVariables.worldMap.worldSize-1) {

        if (gameVariables.worldMap.WorldTerritoryMap [unitX][unitY+1] == civilizationIndex+1) {

            return true;

        }

    }

    return false;

}
