#include "GameUpdater.h"
#include "sharedMethods.h"

#include <math.h>
#include <iostream>

namespace GameUpdater {

void updateForCivilization (int civilizationIndex, GameVariables &gameVariables, AI ai) {

    updateEvents (civilizationIndex, gameVariables);
    updateResources (civilizationIndex, gameVariables);
    updateCivilizationHappiness (civilizationIndex, gameVariables);
    updateTrades (civilizationIndex, gameVariables, ai);
    updateGoldPerTurn (civilizationIndex, gameVariables);
    updateEffectsFromUnitUpkeep (civilizationIndex, gameVariables);

}

void updateResources (int civilizationIndex, GameVariables &gameVariables) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldResourceMap[i][j].ResourceCode != 0 && gameVariables.worldMap.WorldTerritoryMap[i][j] == civilizationIndex+1) {

                gameVariables.Civilizations[civilizationIndex].resources.push_back(gameVariables.worldMap.WorldResourceMap[i][j]);

            }

        }

    }

}

void updateCities (GameVariables &gameVariables) {

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

bool canCityExpand (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {


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

void expandCityTerritory (int cityIndex, GameVariables &gameVariables) {

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

void updateCityProductionModifier (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

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

void updateCityUnitProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    gameVariables.Cities[cityIndex].Production += (gameVariables.Cities[cityIndex].ProductionFromTiles + gameVariables.Cities[cityIndex].ProductionPerTurn)*gameVariables.Cities[cityIndex].ProductionModifier;

    if (gameVariables.Cities[cityIndex].Production >= gameVariables.Cities[cityIndex].unitBeingProduced.productionCost
    && gameVariables.Cities[cityIndex].isProducing == true) {

        sharedMethods::deployUnit (gameVariables.Cities[cityIndex].unitBeingProduced, cityIndex, civilizationIndex, gameVariables);

        gameVariables.Cities[cityIndex].Production -= gameVariables.Cities[cityIndex].unitBeingProduced.productionCost;

        gameVariables.Cities[cityIndex].isProducing = false;
        gameVariables.Cities[cityIndex].isProducingUnit = false;

        std::cout << gameVariables.Cities[cityIndex].cityName << " finished its production!" << std::endl;

    }

}

void updateCityBuildingProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    gameVariables.Cities[cityIndex].Production += (gameVariables.Cities[cityIndex].ProductionFromTiles + gameVariables.Cities[cityIndex].ProductionPerTurn)*gameVariables.Cities[cityIndex].ProductionModifier;

    if (gameVariables.Cities[cityIndex].Production >= gameVariables.Cities[cityIndex].buildingBeingProduced.productionCost && gameVariables.Cities[cityIndex].isProducing == true) {

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
        gameVariables.Cities[cityIndex].Production -= gameVariables.Cities[cityIndex].buildingBeingProduced.productionCost;

        gameVariables.Cities[cityIndex].isProducing = false;
        gameVariables.Cities[cityIndex].isProducingUnit = false;

        std::cout << gameVariables.Cities[cityIndex].cityName << " finished its production!" << std::endl;

    }

}

void updateGoldPerTurn (int civilizationIndex, GameVariables &gameVariables) {

    int gpt = 0;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) {

            gpt += gameVariables.Cities[i].GoldPerTurn;
            gpt += gameVariables.Cities[i].GoldPerTurnFromCity;

        }

    }

    for (unsigned int i = 0; i < gameVariables.Units.size(); i++) {

        if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

            gpt -= gameVariables.Civilizations[civilizationIndex].upkeepCostPerUnit;

        }

    }

    gameVariables.Civilizations[civilizationIndex].GoldPerTurn = gpt;

}

void updateEffectsFromUnitUpkeep (int civilizationIndex, GameVariables &gameVariables) {

    int requiredUpkeep = sharedMethods::returnBaseUnitUpkeep (civilizationIndex, gameVariables),
        actualUpkeep = gameVariables.Civilizations[civilizationIndex].upkeepCostPerUnit;

    if (actualUpkeep < requiredUpkeep) {

        gameVariables.Civilizations[civilizationIndex].warSupportPercentage = gameVariables.Civilizations[civilizationIndex].baseWarSupportPercentage - 10;

        gameVariables.Civilizations[civilizationIndex].unitAttackModifier = 0.65;
        gameVariables.Civilizations[civilizationIndex].unitDefenseModifier = 0.65;

    } else if (actualUpkeep == requiredUpkeep) {

        gameVariables.Civilizations[civilizationIndex].warSupportPercentage = gameVariables.Civilizations[civilizationIndex].baseWarSupportPercentage;

        gameVariables.Civilizations[civilizationIndex].unitAttackModifier = 1.00;
        gameVariables.Civilizations[civilizationIndex].unitDefenseModifier = 1.00;

    } else {

        gameVariables.Civilizations[civilizationIndex].warSupportPercentage = gameVariables.Civilizations[civilizationIndex].baseWarSupportPercentage + 5;

        gameVariables.Civilizations[civilizationIndex].unitAttackModifier = 1.15;
        gameVariables.Civilizations[civilizationIndex].unitDefenseModifier = 1.15;

    }

}

void updateTrades (int civilizationIndex, GameVariables &gameVariables, AI ai) {

    const std::string resourceNames[25] = {"None","Horses","Iron","Rubber","Copper","Pearls","Crab","Whales","Salt","Spices","Stone","Marble","Cocoa","Sheep","Cattle","Diamonds","Nutmeg","Ginger","Silk","Dyes","Citrus","Ivory","Furs","Silver","Gold"};

    for (unsigned int i = 0; i < gameVariables.trades.size(); i++) {

        if (gameVariables.trades[i].recipientIndex == civilizationIndex && gameVariables.Civilizations[civilizationIndex].playedByHumans == true) {

            std::cout << gameVariables.Civilizations[gameVariables.trades[i].traderIndex].CivName << " has requested a trade. " << std::endl;

            std::cout << "= YOUR ITEMS =" << std::endl;

            std::cout << "GOLD: " << gameVariables.trades[i].goldSumFromRecipient << "\nGPT: " << gameVariables.trades[i].GPTFromRecipient << std::endl;

            std::cout << "RESOURCES: " << std::endl;

            for (int j = 0; j < gameVariables.trades[i].resourcesFromRecipient.size(); j++) {

                std::cout << resourceNames[gameVariables.trades[i].resourcesFromRecipient[j].ResourceCode] << std::endl;

            }

            std::cout << "= THEIR ITEMS =" << std::endl;

            std::cout << "GOLD: " << gameVariables.trades[i].goldSumFromTrader << "\nGPT: " << gameVariables.trades[i].GPTFromTrader << std::endl;

            std::cout << "RESOURCES: " << std::endl;

            for (int j = 0; j < gameVariables.trades[i].resourcesFromTrader.size(); j++) {

                std::cout << resourceNames[gameVariables.trades[i].resourcesFromTrader[j].ResourceCode] << std::endl;

            }

            std::cout << "[A]CCEPT OR [D]ENY?" << std::endl;

            char Choice;

            std::cin >> Choice;

            if (Choice == 'a' || Choice == 'A') {

                Event tradeAccepted;

                tradeAccepted.EventName = "Trade Accepted";

                tradeAccepted.EventMessage = "Your trade request has been accepted.";

                tradeAccepted.targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeAccepted);


                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold -= gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold += gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold -= gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold += gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].GoldPerTurn -= gameVariables.trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn += gameVariables.trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn -= gameVariables.trades[i].GPTFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].GoldPerTurn += gameVariables.trades[i].GPTFromRecipient;


                for (unsigned int k = 0; k < gameVariables.trades[i].resourcesFromTrader.size(); k++) {

                    gameVariables.Civilizations[civilizationIndex].resources.push_back (gameVariables.trades[i].resourcesFromTrader[k]);

                    gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.erase (gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.begin() + k);


                }

                for (unsigned int k = 0; k < gameVariables.trades[i].resourcesFromRecipient.size(); k++) {

                    gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.push_back (gameVariables.trades[i].resourcesFromRecipient[k]);

                    gameVariables.Civilizations[civilizationIndex].resources.erase (gameVariables.Civilizations[civilizationIndex].resources.begin() + k);


                }

            } else {

                Event tradeDenied;

                tradeDenied.EventName = "Trade Denied";

                tradeDenied.EventMessage = "Your trade request has been denied.";

                tradeDenied.targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeDenied);


            }

            gameVariables.trades.erase (gameVariables.trades.begin() + i);

        }

        if (gameVariables.trades[i].recipientIndex == civilizationIndex && gameVariables.Civilizations[civilizationIndex].playedByHumans == false) {

            if (ai.returnTradeValue (gameVariables.trades[i], gameVariables) >= 0) {

                Event tradeAccepted;

                tradeAccepted.EventName = "Trade Accepted";

                tradeAccepted.EventMessage = "Your trade request has been accepted.";

                tradeAccepted.targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeAccepted);

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold -= gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold += gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold -= gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold += gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].GoldPerTurn -= gameVariables.trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn += gameVariables.trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn -= gameVariables.trades[i].GPTFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].GoldPerTurn += gameVariables.trades[i].GPTFromRecipient;

                for (unsigned int k = 0; k < gameVariables.trades[i].resourcesFromTrader.size(); k++) {

                    gameVariables.Civilizations[civilizationIndex].resources.push_back (gameVariables.trades[i].resourcesFromTrader[k]);

                    gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.erase (gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.begin() + k);


                }

                for (unsigned int k = 0; k < gameVariables.trades[i].resourcesFromRecipient.size(); k++) {

                    gameVariables.Civilizations[gameVariables.trades[i].traderIndex].resources.push_back (gameVariables.trades[i].resourcesFromRecipient[k]);

                    gameVariables.Civilizations[civilizationIndex].resources.erase (gameVariables.Civilizations[civilizationIndex].resources.begin() + k);


                }

            } else {

                Event tradeDenied;

                tradeDenied.EventName = "Trade Denied";

                tradeDenied.EventMessage = "Your trade request has been denied.";

                tradeDenied.targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeDenied);

            }

            gameVariables.trades.erase (gameVariables.trades.begin() + i);

        }

    }

}


void updateCivilizationHappiness (int civilizationIndex, GameVariables &gameVariables) {

    int baseHappiness = 75;

    for (unsigned int i = 0; i < gameVariables.Civilizations[i].resources.size(); i++) {

        baseHappiness += 3;

    }

    for (unsigned int j = 0; j < gameVariables.Cities.size(); j++) {

        if (gameVariables.Cities[j].parentIndex == civilizationIndex) {

            baseHappiness -= gameVariables.Cities[j].Population;

        }

    }

    gameVariables.Civilizations[civilizationIndex].Happiness = baseHappiness;

}

void updateEvents (int civilizationIndex, GameVariables &gameVariables) {

    for (unsigned int e = 0; e < gameVariables.gameEvents.size(); e++) {

        gameVariables.gameEvents[e].listen(civilizationIndex, gameVariables.Civilizations);

        if (gameVariables.gameEvents[e].hasBeenTriggered == true) {

            gameVariables.gameEvents.erase(gameVariables.gameEvents.begin() + e);

        }

    }

}

void UpdateCivilizationExploredTerritory (int civilizationIndex, GameVariables &gameVariables) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[i][j] == (civilizationIndex+1)) {

                for (int k = -4; k < 5; k++) {

                    for (int l = -4; l < 5; l++) {

                        if (sharedMethods::getDistance(i,j,i+k,j+l) < 3 && gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i+k][j+l] == 0) { gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i+k][j+l] = 1; }

                    }

                }

            }

        }

    }
}

void UpdateAllUnitsMovement (GameVariables &gameVariables) {


        for (unsigned int a = 0; a < gameVariables.UnitsInGame.size(); a++) {

            if (gameVariables.UnitsInGame[a].isTraining == false) {

                gameVariables.UnitsInGame[a].movementPoints = gameVariables.UnitsInGame[a].maxMovePoints;

            } else {

                gameVariables.UnitsInGame[a].movementPoints = 0;

            }

        }

}

bool unitIsNotAlreadyUnlocked (int civilizationIndex, std::string unitName, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

        if (unitName == gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i]) {

            return false;

        }

    }

    return true;

}

void unlockUnitsFromResearchCompletion (Research research, int civilizationIndex, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < research.unlockableUnits.size(); i++) {

        if (unitIsNotAlreadyUnlocked (civilizationIndex, research.unlockableUnits[i], gameVariables)) {

            gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.push_back (research.unlockableUnits[i]);

        }

    }

}

void promoteUnitsFromResearchCompletion (Research research, int civilizationIndex, GameVariables &gameVariables) {

    if (research.unlockablePromotion != "none") {

        if (research.unlockablePromotion == "coastal_embark") {

            for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

                if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

                    gameVariables.UnitsInGame[i].domain.canCoastalEmbark = true;

                }

            }

        } else if (research.unlockablePromotion == "ocean_embark") {

            for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

                if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

                    gameVariables.UnitsInGame[i].domain.canCrossOceans = true;

                }

            }

        }

    }

}


void updateResearch (int civilizationIndex, GameVariables &gameVariables) {

    if (sharedMethods::isResearchComplete(civilizationIndex, gameVariables)) {

        gameVariables.Civilizations[civilizationIndex].learnedTechnologies.push_back(gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName);

        gameVariables.Civilizations[civilizationIndex].researchPoints -= gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.scienceCostToLearnResearch;

        unlockUnitsFromResearchCompletion (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched, civilizationIndex, gameVariables);

        promoteUnitsFromResearchCompletion (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched, civilizationIndex, gameVariables);

        std::cout << gameVariables.Civilizations[civilizationIndex].CivName << " finished researching " << gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName << "!" << std::endl;

        if (sharedMethods::getResearchIndexByName(civilizationIndex, gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName, gameVariables) > -1) {

            gameVariables.Civilizations[civilizationIndex].technologiesToResearch.erase (gameVariables.Civilizations[civilizationIndex].technologiesToResearch.begin() + sharedMethods::getResearchIndexByName(civilizationIndex, gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName, gameVariables));

        }

        gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName = "";


    }

}

}
