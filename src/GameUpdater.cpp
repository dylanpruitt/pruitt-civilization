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

        double FoodModifier = 1.0;

        if (gameVariables.Civilizations[gameVariables.Cities[w].parentIndex].focus == "Population") {

            FoodModifier = 1.10;

        }

        gameVariables.Cities[w].FoodSurplus += FoodModifier * ((gameVariables.Cities[w].FoodPerTurnFromCity + gameVariables.Cities[w].FoodPerTurnFromTiles) - (gameVariables.Cities[w].Population*2));

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

    if (gameVariables.Civilizations[civilizationIndex].GoldPerTurn >= 75) {

        tempModifier += 15;

    } else if (gameVariables.Civilizations[civilizationIndex].GoldPerTurn >= 15) {

        tempModifier += 5;

    } else if (gameVariables.Civilizations[civilizationIndex].GoldPerTurn >= -5 && gameVariables.Civilizations[civilizationIndex].GoldPerTurn < 0) {

        tempModifier -= 5;

    } else {

        tempModifier -= 15;

    }

    if (gameVariables.Civilizations[civilizationIndex].Gold <= 0 && gameVariables.Civilizations[civilizationIndex].GoldPerTurn <= 0) {

        tempModifier -= 85;

    }

    if (sharedMethods::civilizationHasTechnology(civilizationIndex, "Masonry", gameVariables)) {

        tempModifier += 5;

    }

    if (gameVariables.Cities[cityIndex].isProducing == true && gameVariables.Cities[cityIndex].isProducingUnit == false
        && gameVariables.Civilizations[civilizationIndex].focus == "Construction") {

        tempModifier += 15;

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

        gameVariables.Cities[cityIndex].lastProductionType = "unit";

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

        gameVariables.Cities[cityIndex].lastProductionType = "building";

    }

}

void updateCityRevoltChances (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    int baseChance = 1;

    if (gameVariables.Cities[cityIndex].isCapital) {

        baseChance -= 100;

    }

    if (gameVariables.Civilizations[civilizationIndex].Happiness <= 40) {

        baseChance += (41 - gameVariables.Civilizations[civilizationIndex].Happiness);

    }

    gameVariables.Cities[cityIndex].chanceOfRevoltingPerTurnOutOf100 = baseChance;

}

void startRevolt (int cityIndex, int civilizationIndex, GameVariables &gameVariables) {

    Civilization revolting_civilization;

    revolting_civilization.CivName = gameVariables.Cities[cityIndex].cityName;
    revolting_civilization.CivilizationAdjective = gameVariables.Cities[cityIndex].cityName;

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].learnedTechnologies.size(); i++) {

        revolting_civilization.learnedTechnologies.push_back (gameVariables.Civilizations[civilizationIndex].learnedTechnologies[i]);

    }

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].technologiesToResearch.size(); i++) {

        Research temp_research;

        temp_research = gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i];

        revolting_civilization.technologiesToResearch.push_back (temp_research);

    }

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

         revolting_civilization.AvailableUnitsToCreate.push_back (gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i]);

    }

    for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

        revolting_civilization.relationsWithOtherCivilizations.push_back (0);

        gameVariables.Civilizations[i].relationsWithOtherCivilizations.push_back (0);

    }

    for (unsigned int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (unsigned int j = 0; j < gameVariables.worldMap.worldSize * 4; j++) {

            if (sharedMethods::getDistance (gameVariables.Cities[cityIndex].position.x, gameVariables.Cities[cityIndex].position.y, i, j) < 3) {

                revolting_civilization.WorldExplorationMap[i][j] = 1;

            } else {

                 revolting_civilization.WorldExplorationMap[i][j] = 0;

            }

        }

    }

    revolting_civilization.startingX = gameVariables.Cities[cityIndex].position.x;
    revolting_civilization.startingY = gameVariables.Cities[cityIndex].position.y;

    revolting_civilization.rgbValues[0] = 200, revolting_civilization.rgbValues[1] = 200, revolting_civilization.rgbValues[2] = 200;

    revolting_civilization.warSupportPercentage = gameVariables.Cities[cityIndex].chanceOfRevoltingPerTurnOutOf100*2;

    gameVariables.Civilizations.push_back (revolting_civilization);

    gameVariables.Cities[cityIndex].parentIndex = gameVariables.Civilizations.size () - 1;
    gameVariables.Cities[cityIndex].isCapital = true;

    for (unsigned int i = -1; i < 2; i++) {

        for (unsigned int j = -1; j < 2; j++) {

            gameVariables.worldMap.WorldTerritoryMap [gameVariables.Cities[cityIndex].position.x+i][gameVariables.Cities[cityIndex].position.y+j]
            = gameVariables.Civilizations.size ();

        }

    }


    War city_revolt;

    city_revolt.name = gameVariables.Civilizations[civilizationIndex].CivilizationAdjective;

    city_revolt.offenderCivilizationIndices.push_back (gameVariables.Civilizations.size () - 1);

    city_revolt.defenderCivilizationIndices.push_back (civilizationIndex);

    gameVariables.wars.push_back (city_revolt);



    Event *revolt_started = new Event;

    revolt_started->EventName = "Revolt Started";

    revolt_started->EventMessage = "The city " + revolting_civilization.CivName + " has revolted against us!";

    revolt_started->targetCivilizationIndex = civilizationIndex;

    revolt_started->ResponseChoices.push_back ("To arms!");

    gameVariables.gameEvents.push_back (revolt_started);


}

void updateRevolts (GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        updateCityRevoltChances (i, gameVariables.Cities[i].parentIndex, gameVariables);

        int temp_random_number = rand () % 99 + 1;

        if (temp_random_number <= gameVariables.Cities[i].chanceOfRevoltingPerTurnOutOf100) {

            startRevolt (i, gameVariables.Cities[i].parentIndex, gameVariables);

        }

    }

}

void updateGoldPerTurn (int civilizationIndex, GameVariables &gameVariables) {

    int gpt = 0; double gptModifier = 1.0;

    if (gameVariables.Civilizations[civilizationIndex].focus == "Economic") {

        gptModifier = 1.15;

    }

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) {

            gpt += gameVariables.Cities[i].GoldPerTurn;
            gpt += gameVariables.Cities[i].GoldPerTurnFromCity;

            gpt *= gptModifier;

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

                Event *tradeAccepted = new Event;

                tradeAccepted->EventName = "Trade Accepted";

                tradeAccepted->EventMessage = "Your trade request has been accepted.";

                tradeAccepted->targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeAccepted);


                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold -= gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold += gameVariables.trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold -= gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].Gold += gameVariables.trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[gameVariables.trades[i].traderIndex].GoldPerTurn -= gameVariables.trades[i].goldSumFromTrader;

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

                Event *tradeDenied = new Event;

                tradeDenied->EventName = "Trade Denied";

                tradeDenied->EventMessage = "Your trade request has been denied.";

                tradeDenied->targetCivilizationIndex = gameVariables.trades[i].traderIndex;

                gameVariables.gameEvents.push_back (tradeDenied);


            }

            gameVariables.trades.erase (gameVariables.trades.begin() + i);

        }

        if (gameVariables.trades[i].recipientIndex == civilizationIndex && gameVariables.Civilizations[civilizationIndex].playedByHumans == false) {

            if (ai.returnTradeValue (gameVariables.trades[i], gameVariables) >= 0) {

                Event *tradeAccepted = new Event;

                tradeAccepted->EventName = "Trade Accepted";

                tradeAccepted->EventMessage = "Your trade request has been accepted.";

                tradeAccepted->targetCivilizationIndex = gameVariables.trades[i].traderIndex;

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

                Event *tradeDenied = new Event;

                tradeDenied->EventName = "Trade Denied";

                tradeDenied->EventMessage = "Your trade request has been denied.";

                tradeDenied->targetCivilizationIndex = gameVariables.trades[i].traderIndex;

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

    if (gameVariables.Civilizations[civilizationIndex].isAtWar) { baseHappiness -= 30; }

    gameVariables.Civilizations[civilizationIndex].Happiness = baseHappiness;

}

void updateEvents (int civilizationIndex, GameVariables &gameVariables) {

    for (unsigned int e = 0; e < gameVariables.gameEvents.size(); e++) {

        gameVariables.gameEvents[e]->listen(civilizationIndex, gameVariables);

        if (gameVariables.gameEvents[e]->hasBeenTriggered == true) {

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

void removeEliminatedCivilizations (GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

        if (!civilizationStillHasLand (i, gameVariables)) {

            std::cout << gameVariables.Civilizations[i].CivName << " was eliminated!" << std::endl;

            gameVariables.Civilizations.erase (gameVariables.Civilizations.begin() + i);

            cleanupAfterCivilizationRemoval (i, gameVariables);

        }

    }

}

void cleanupAfterCivilizationRemoval (int eliminatedCivilizationIndex, GameVariables &gameVariables) {

    for (unsigned int j = 0; j < gameVariables.trades.size(); j++) {

        if (gameVariables.trades[j].traderIndex == eliminatedCivilizationIndex
            && gameVariables.trades[j].recipientIndex == eliminatedCivilizationIndex) {

            gameVariables.trades.erase (gameVariables.trades.begin () + j);

        } else {

            if (gameVariables.trades[j].traderIndex > eliminatedCivilizationIndex) {

                gameVariables.trades[j].traderIndex--;

            }

            if (gameVariables.trades[j].recipientIndex > eliminatedCivilizationIndex) {

                gameVariables.trades[j].recipientIndex--;

            }

        }

    }

    for (unsigned int j = 0; j < gameVariables.activeLoans.size(); j++) {

        if (gameVariables.activeLoans[j].creditorCivilizationIndex == eliminatedCivilizationIndex
            && gameVariables.activeLoans[j].debtorCivilizationIndex == eliminatedCivilizationIndex) {

            gameVariables.activeLoans.erase (gameVariables.activeLoans.begin () + j);

        } else {

            if (gameVariables.activeLoans[j].creditorCivilizationIndex > eliminatedCivilizationIndex) {

                gameVariables.activeLoans[j].creditorCivilizationIndex--;

            }

            if (gameVariables.activeLoans[j].debtorCivilizationIndex > eliminatedCivilizationIndex) {

                gameVariables.activeLoans[j].debtorCivilizationIndex--;

            }

        }

    }

    for (unsigned int j = 0; j < gameVariables.wars.size(); j++) {

        for (unsigned int i = 0; i < gameVariables.wars[j].offenderCivilizationIndices.size(); i++) {

            if (gameVariables.wars[j].offenderCivilizationIndices[i] > eliminatedCivilizationIndex) {

                gameVariables.wars[j].offenderCivilizationIndices[i]--;

            } else if (gameVariables.wars[j].offenderCivilizationIndices[i] == eliminatedCivilizationIndex) {

                gameVariables.wars[j].offenderCivilizationIndices.erase (gameVariables.wars[j].offenderCivilizationIndices.begin () + i);

            }

        }

        for (unsigned int i = 0; i < gameVariables.wars[j].defenderCivilizationIndices.size(); i++) {

            if (gameVariables.wars[j].defenderCivilizationIndices[i] > eliminatedCivilizationIndex) {

                gameVariables.wars[j].defenderCivilizationIndices[i]--;

            } else if (gameVariables.wars[j].defenderCivilizationIndices[i] == eliminatedCivilizationIndex) {

                gameVariables.wars[j].defenderCivilizationIndices.erase (gameVariables.wars[j].defenderCivilizationIndices.begin () + i);

            }

        }

    }

    for (unsigned int j = 0; j < gameVariables.alliances.size(); j++) {

        for (unsigned int i = 0; i < gameVariables.alliances[j].memberCivilizationIndices.size(); i++) {

            if (gameVariables.alliances[j].memberCivilizationIndices[i] > eliminatedCivilizationIndex) {

                gameVariables.alliances[j].memberCivilizationIndices[i]--;

            } else if (gameVariables.alliances[j].memberCivilizationIndices[i] == eliminatedCivilizationIndex) {

                gameVariables.alliances[j].memberCivilizationIndices.erase (gameVariables.alliances[j].memberCivilizationIndices.begin () + i);

            }

        }

    }

    for (unsigned int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (unsigned int j = 0; j < gameVariables.worldMap.worldSize; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[i][j]-1 > eliminatedCivilizationIndex) {

                gameVariables.worldMap.WorldTerritoryMap[i][j]--;

            }

        }

    }

}

bool civilizationStillHasLand (int civilizationIndex, GameVariables &gameVariables) {

    int numberOfOwnedCities = 0;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) { numberOfOwnedCities++; }

    }

    if (numberOfOwnedCities > 0) {

        return true;

    } else {

        return false;

    }

}

}
