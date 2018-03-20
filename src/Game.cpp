#include "Game.h"
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <thread>
#include <chrono>

#include "Unit.h"
#include "Trade.h"
#include "Resource.h"
#include "MapGenerator.h"
#include "textRenderer.h"
#include "sharedMethods.h"
#include "AI.h"
#include "Event.h"

using namespace std;

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}

textRenderer renderer;
AI ai;

GameVariables gameVariables;

std::vector<Civilization> civilizations; std::vector<Event> gameEvents; std::vector<Trade> trades;

const std::string resourceNames[25] = {"None","Horses","Iron","Rubber","Copper","Pearls","Crab","Whales","Salt","Spices","Stone","Marble","Cocoa","Sheep","Cattle","Diamonds","Nutmeg","Ginger","Silk","Dyes","Citrus","Ivory","Furs","Silver","Gold"};

bool loopVariable = true;

    const int cityExpansionLimit = 3;

int turnNumber = 0;
bool gameLoopVariable = true;

enum characterActionCodes : char {
    EXPLORATION = 'x',
    UNITS = 'u',
    MOVE = 'm',
    BUY = 'b',
    SPECIAL = 's',
    END = 'e',
};

enum focuses : int {
    POPULATION,
    PRODUCTION,
    GOLD,
    SCIENCE
};

int Game::returnTerritoryTiles (int x, int y, int Civ_index) {

    int TerritoryTiles = 0; /// number of alive tiles

    if (x > 0){
        if (gameVariables.worldMap.WorldTerritoryMap[x-1][y] == (Civ_index+1)){
            TerritoryTiles++;
        }
    }

    if (y > 0){
        if (gameVariables.worldMap.WorldTerritoryMap[x][y-1] == (Civ_index+1)){
            TerritoryTiles++;
        }
    }

    if (y < (gameVariables.worldMap.worldSize*4-1)){
        if (gameVariables.worldMap.WorldTerritoryMap[x][y+1] == (Civ_index+1)){
            TerritoryTiles++;
        }
    }

    if (x < (gameVariables.worldMap.worldSize-1)){
        if (gameVariables.worldMap.WorldTerritoryMap[x+1][y] == (Civ_index+1)){
            TerritoryTiles++;
        }
    }

    return TerritoryTiles;
}

bool Game::isLandTile (int x, int y) {

    if (gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::COAST && gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::OCEAN) {

        return true;

    } else {

        return false;

    }

}

int Game::returnLandTiles (int x, int y){

    int LandTiles = 0; /// number of alive tiles

    if (x > 0) {

        if (isLandTile(x-1,y)) {

            LandTiles++;

        }

    }

    if (y > 0) {

        if (isLandTile(x,y-1)) {

            LandTiles++;

        }

    }

    if (x < gameVariables.worldMap.worldSize-1) {

        if (isLandTile(x+1,y)) {

            LandTiles++;

        }

    }

    if (y < gameVariables.worldMap.worldSize*4-1) {

        if (isLandTile(x+1,y)) {

            LandTiles++;

        }

    }

    if (x > 0 && y > 0) {

        if (isLandTile(x-1,y-1)) {

            LandTiles++;

        }

    }

    if (x > 0 && y < (gameVariables.worldMap.worldSize*4-1)) {

        if (isLandTile(x-1,y+1)) {

            LandTiles++;

        }

    }

    if (x < (gameVariables.worldMap.worldSize-1) && y > 0) {

        if (isLandTile(x+1,y-1)) {

            LandTiles++;

        }

    }

    if (x < (gameVariables.worldMap.worldSize-1) && y < (gameVariables.worldMap.worldSize*4-1)) {

        if (isLandTile(x+1,y+1)) {

            LandTiles++;

        }

    }

    return LandTiles;
}

int Game::getTerritory (int Civ_index) {

    int numberOfTiles = 0;

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[i][j] == Civ_index) {
                numberOfTiles++;
            }

        }

    }

    return numberOfTiles;

}

void Game::setupWorld () {

    loadUnitsFromFile("units.sav");
    loadBuildingsFromFile("buildings.sav");

    MapGenerator mapGen;
    std::cout << "What type of world do you want to be generated?\n[1] Continents\n[2] Pangaea" << std::endl;

    int choice = 1; int AmountOfCivilizations = 5;

    std::cin >> choice; /*flag (r: add bindIntinput here*/

    switch (choice) {

        case 1:
            mapGen.generateWorld_Continents(gameVariables.worldMap);
        break;
        case 2:
            mapGen.generateWorld_Pangaea(gameVariables.worldMap);
        break;
        default:
            mapGen.generateWorld_Continents(gameVariables.worldMap);
        break;

    }

    std::cout << "How many Civilizations do you want? The maximum is 16." << std::endl;

    AmountOfCivilizations = sharedMethods::bindIntegerInputToRange(2,16,4);

    for (int i = 0; i < AmountOfCivilizations; i++) {

        generateCiv(i);

    }

    setupDiplomacy();

    renderer.worldMap = gameVariables.worldMap;

}

void Game::setupDiplomacy () {

    for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

        for (unsigned int j = 0; j < gameVariables.Civilizations.size(); j++) {

            int variantStartDiplomacy = rand() % 50 - 25;
            gameVariables.Civilizations[i].relationsWithOtherCivilizations.push_back(variantStartDiplomacy);

        }

    }

}

void Game::foundCity (int x, int y, int CivilizationIndex) {

    City city;

    city.position.x = x;
    city.position.y = y;

    int cityNameIndex = rand() % gameVariables.Civilizations[CivilizationIndex].cityNames.size();

    city.cityName = gameVariables.Civilizations[CivilizationIndex].cityNames[cityNameIndex];

    city.AvailableBuildingsToCreate.push_back("Granary");
    city.AvailableBuildingsToCreate.push_back("Library");
    city.AvailableBuildingsToCreate.push_back("Market");

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {
        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {
            if (sharedMethods::getDistance(i,j,x,y) <= 1) {
                gameVariables.worldMap.WorldTerritoryMap[i][j] = CivilizationIndex+1;
            }
        }
    }

    city.parentIndex = CivilizationIndex;

    gameVariables.Cities.push_back(city);

    assignWorkByPopulation(gameVariables.Cities.size() - 1, false);

}

int returnUnitAtPositionByIndex (int x, int y) {

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].position.x == x && gameVariables.UnitsInGame[i].position.y == y) {

            return i;

        }

    }

    return -1;
}

int returnCivMilitaryPower (int civ_index, std::vector<Civilization> civs) {

    int militarypower = 0;

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civ_index) {

            if (civs[civ_index].LeaderTrait != "Brave") {
                militarypower += (gameVariables.UnitsInGame[i].combat * gameVariables.UnitsInGame[i].combat);
            } else {
                militarypower += ((gameVariables.UnitsInGame[i].combat*1.15) * (gameVariables.UnitsInGame[i].combat*1.15));
            }

        }

    }

    return militarypower;

}

void Game::loadTechnologiesFromFile (std::string filename, int civilizationIndex) {

    std::string line;
    ifstream file (filename);

    if (file.is_open())  {

    int techSize = 0, prereqSize = 0, numberOfUnlockableUnits = 0; Research temp;

    file >> techSize;

        for (int i = 0; i < techSize; i++) {

           temp.unlockableUnits.clear();
           temp.prerequisiteTechnologiesRequired.clear();

           file >> temp.researchName;
           file >> temp.scienceCostToLearnResearch;
           file >> prereqSize;

           for (int j = 0; j < prereqSize; j++) {

                std::string x;
                file >> x;
                temp.prerequisiteTechnologiesRequired.push_back(x);

           }

           file >> temp.aiFocus_offense;
           file >> temp.aiFocus_defense;
           file >> temp.aiFocus_economic;
           file >> temp.aiFocus_population;
           file >> temp.aiFocus_production;
           file >> temp.aiFocus_diplomatic;
           file >> temp.aiFocus_scientific;
           file >> temp.aiFocus_exploration;
           file >> temp.aiFocus_religion;
           file >> temp.aiFocus_overall_importance;

           file >> numberOfUnlockableUnits;

           for (int j = 0; j < numberOfUnlockableUnits; j++) {

                std::string x;
                file >> x;
                temp.unlockableUnits.push_back(x);

           }

           gameVariables.Civilizations[civilizationIndex].technologiesToResearch.push_back(temp);

    }

    file.close();

    } else cout << "Unable to open file";

}

void Game::loadUnitsFromFile (std::string filename) {

    std::string line;
    ifstream file (filename);

    if (file.is_open())  {

    int unitSize;
    Unit temp;

    file >> unitSize;

    for (int i = 0; i < unitSize; i++) {

           file >> temp.name;
           file >> temp.health; temp.maxHealth = temp.health;
           file >> temp.combat;
           file >> temp.maxMovePoints;
           file >> temp.terrainMoveCost;
           file >> temp.productionCost;
           file >> temp.goldCost;

           file >> line; /// just used to skip over lines, need to fix

           file >> temp.aiFocus_defense;
           file >> temp.aiFocus_economic;
           file >> temp.aiFocus_exploration;
           file >> temp.aiFocus_offense;
           file >> temp.aiFocus_overall_importance;
           file >> temp.isRanged;

           if (temp.isRanged == true) {

              file >> temp.rangedCombat;

           }

           file >> line; /// just used to skip over lines, need to fix

           file >> temp.grasslandModifier.attackModifier;
           file >> temp.grasslandModifier.defenseModifier;
           file >> temp.forestModifier.attackModifier;
           file >> temp.forestModifier.defenseModifier;
           file >> temp.mountainModifier.attackModifier;
           file >> temp.mountainModifier.defenseModifier;
           file >> temp.snowModifier.attackModifier;
           file >> temp.snowModifier.defenseModifier;
           file >> temp.desertModifier.attackModifier;
           file >> temp.desertModifier.defenseModifier;

           gameVariables.Units.push_back(temp);

    }

    file.close();

    } else cout << "Unable to open file";

}

void Game::loadBuildingsFromFile (std::string filename) {

    std::string line;
    ifstream file (filename);

    if (file.is_open())  {

    int buildingSize;
    Building temp;

    file >> buildingSize;

    for (int i = 0; i < buildingSize; i++) {

           file >> temp.name;
           file >> temp.FoodYield;
           file >> temp.ProductionYield;
           file >> temp.GoldYield;
           file >> temp.ScienceYield;
           file >> temp.FaithYield;

           file >> temp.aiFocus_defense;
           file >> temp.aiFocus_economic;
           file >> temp.aiFocus_exploration;
           file >> temp.aiFocus_offense;
           file >> temp.aiFocus_overall_importance;
           file >> temp.aiFocus_population;
           file >> temp.aiFocus_production;
           file >> temp.aiFocus_religion;
           file >> temp.aiFocus_scientific;
           file >> temp.GoldCost;
           file >> temp.ProductionCost;

           gameVariables.Buildings.push_back(temp);

    }

    file.close();

    } else cout << "Unable to open file";

}

void Game::loadCivilizationsFromFile (std::string filename, std::vector<Civilization> &civs) {

    std::string line;
    ifstream file (filename);

    if (file.is_open())  {

    int civsSize;
    Civilization temp;

    file >> civsSize;

    for (int i = 0; i < civsSize; i++) {

           file >> temp.CivName;

           file >> temp.ExpansionRate;
           file >> temp.ScienceRate;

           int freeTechSize = 0;

           file >> freeTechSize;

           for (int j = 0; j < freeTechSize; j++) {

                std::string tempString;
                file >> tempString;

                temp.learnedTechnologies.push_back(tempString);

           }

           file >> temp.aiFocus_defense;
           file >> temp.aiFocus_diplomatic;
           file >> temp.aiFocus_economic;
           file >> temp.aiFocus_exploration;
           file >> temp.aiFocus_offense;
           file >> temp.aiFocus_population;
           file >> temp.aiFocus_production;
           file >> temp.aiFocus_religion;
           file >> temp.aiFocus_scientific;
           file >> temp.ai_fairness;

           int cityNamesSize = 0;

           file >> cityNamesSize;

           for (int j = 0; j < cityNamesSize; j++) {

                std::string tempString;
                file >> tempString;

                temp.cityNames.push_back(tempString);

           }

           for (int j = 0; j < 3; j++) {

                file >> temp.rgbValues[j];

           }

           civs.push_back(temp);

    }

    file.close();

    } else cout << "Unable to open file";

}

void Game::saveGame (std::string filename) {

    std::string line;
    ofstream file (filename);

    if (file.is_open()) {

        /// SAVE WORLD DATA

        file << "mapdata" << "\n";

        for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

            for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

                file << gameVariables.worldMap.featureMap[i][j] << "\n";

                file << gameVariables.worldMap.WorldResourceMap[i][j].ResourceCode << "\n";

                file << gameVariables.worldMap.WorldResourceMap[i][j].AmountOfResource << "\n";

                file << gameVariables.worldMap.WorldTerritoryMap[i][j] << "\n";


            }

        }

        file << "unit data" << "\n";

        file << gameVariables.UnitsInGame.size() << "\n";

        for (int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

            file << " > unit #" << i << "\n";

            file << gameVariables.UnitsInGame[i].parentCivilizationIndex << "\n";

            file << gameVariables.UnitsInGame[i].name << "\n";

            file << gameVariables.UnitsInGame[i].position.x << "\n";
            file << gameVariables.UnitsInGame[i].position.y << "\n";

            file << gameVariables.UnitsInGame[i].health << "\n";
            file << gameVariables.UnitsInGame[i].maxHealth << "\n";
            file << gameVariables.UnitsInGame[i].combat << "\n";

            file << gameVariables.UnitsInGame[i].grasslandModifier.attackModifier << "\n";
            file << gameVariables.UnitsInGame[i].grasslandModifier.defenseModifier << "\n";
            file << gameVariables.UnitsInGame[i].grasslandModifier.experience << "\n";

            file << gameVariables.UnitsInGame[i].mountainModifier.attackModifier << "\n";
            file << gameVariables.UnitsInGame[i].mountainModifier.defenseModifier << "\n";
            file << gameVariables.UnitsInGame[i].mountainModifier.experience << "\n";

            file << gameVariables.UnitsInGame[i].forestModifier.attackModifier << "\n";
            file << gameVariables.UnitsInGame[i].forestModifier.defenseModifier << "\n";
            file << gameVariables.UnitsInGame[i].forestModifier.experience << "\n";

            file << gameVariables.UnitsInGame[i].desertModifier.attackModifier << "\n";
            file << gameVariables.UnitsInGame[i].desertModifier.defenseModifier << "\n";
            file << gameVariables.UnitsInGame[i].desertModifier.experience << "\n";

            file << gameVariables.UnitsInGame[i].snowModifier.attackModifier << "\n";
            file << gameVariables.UnitsInGame[i].snowModifier.defenseModifier << "\n";
            file << gameVariables.UnitsInGame[i].snowModifier.experience << "\n";

            file << gameVariables.UnitsInGame[i].maxMovePoints << "\n";

            file << gameVariables.UnitsInGame[i].destinationHasBeenAssigned << "\n";

            file << gameVariables.UnitsInGame[i].isTraining << "\n";


        }

        file << "civ data" << "\n";

        file << gameVariables.Civilizations.size() << "\n";

        for (int i = 0; i < gameVariables.Civilizations.size(); i++) {

            file << gameVariables.Civilizations[i].Gold << "\n";

            file << gameVariables.Civilizations[i].ExpansionRate << "\n";

            file << gameVariables.Civilizations[i].ScienceRate << "\n";

            file << gameVariables.Civilizations[i].playedByHumans << "\n";

        }

        file.close();

    }
}

bool Game::isResearchComplete (int civilizationIndex) {

    if (gameVariables.Civilizations[civilizationIndex].researchPoints >= gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.scienceCostToLearnResearch) {

        return true;

    }

    return false;

}

bool Game::civilizationHasTechnology (int civilizationIndex, std::string techName) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].learnedTechnologies.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].learnedTechnologies[i] == techName) {

            return true;

        }

    }

    return false;

}

void Game::promoteUnitsToAllowCoastalEmbarkment (int civilizationIndex) {

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (civilizationIndex == gameVariables.UnitsInGame[i].parentCivilizationIndex) {

            gameVariables.UnitsInGame[i].canCoastalEmbark = true;

        }

    }

}

bool Game::unitIsNotAlreadyUnlocked (int civilizationIndex, std::string unitName) {

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

        if (unitName == gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i]) {

            return false;

        }

    }

    return true;

}

void Game::unlockUnitsFromResearchCompletion (Research research, int civilizationIndex) {

    for (unsigned int i = 0; i < research.unlockableUnits.size(); i++) {

        if (unitIsNotAlreadyUnlocked (civilizationIndex, research.unlockableUnits[i])) {

            gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.push_back (research.unlockableUnits[i]);

        }

    }

}

void Game::updateResearch (int civilizationIndex) {

    if (isResearchComplete(civilizationIndex)) {

        gameVariables.Civilizations[civilizationIndex].learnedTechnologies.push_back(gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName);

        gameVariables.Civilizations[civilizationIndex].researchPoints -= gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.scienceCostToLearnResearch;

        unlockUnitsFromResearchCompletion (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched, civilizationIndex);

        std::cout << gameVariables.Civilizations[civilizationIndex].CivName << " finished researching " << gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName << "!" << std::endl;

        if (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName == "Embarking") {

            promoteUnitsToAllowCoastalEmbarkment (civilizationIndex);

        }

        if (sharedMethods::getResearchIndexByName(civilizationIndex, gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName, gameVariables) > -1) {

            gameVariables.Civilizations[civilizationIndex].technologiesToResearch.erase (gameVariables.Civilizations[civilizationIndex].technologiesToResearch.begin() + sharedMethods::getResearchIndexByName(civilizationIndex, gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName, gameVariables));

        }

        gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName = "";


    }

}

void Game::checkTileForResource (int civilizationIndex, int x, int y) {

    if (gameVariables.worldMap.WorldResourceMap[x][y].ResourceCode != gameVariables.worldMap.resources::NONE && gameVariables.worldMap.WorldTerritoryMap[x][y] == civilizationIndex+1) {

        gameVariables.Civilizations[civilizationIndex].resources.push_back(gameVariables.worldMap.WorldResourceMap[x][y]);

    }

}

void Game::pickResearchForHumanPlayer (int civilizationIndex) {

    std::vector<int> researchableTechIndices;

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].technologiesToResearch.size(); i++) {

        if (sharedMethods::CivilizationHasPrerequisiteTechs(civilizationIndex,
        gameVariables.Civilizations[civilizationIndex].technologiesToResearch[i].researchName, gameVariables)) {

            researchableTechIndices.push_back(i);

        }

    }

    if (researchableTechIndices.size() > 0) {

        std::cout << "Choose a technology to research:";
        for (unsigned int i = 0; i < researchableTechIndices.size(); i++) {

            std::cout << "\n[" << i << "] " << gameVariables.Civilizations[civilizationIndex].technologiesToResearch[researchableTechIndices[i]].researchName;

        }

        int techIndex = sharedMethods::bindIntegerInputToRange(0, researchableTechIndices.size()-1, 0);

        gameVariables.Civilizations[civilizationIndex].technologyBeingResearched = gameVariables.Civilizations[civilizationIndex].technologiesToResearch[researchableTechIndices[techIndex]];

    } else {

        std::cout << "You have nothing left to research!" << std::endl;

    }

}

int Game::getCivilizationPopulation (int civilizationIndex) {

    int population = 0;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) {

            population += gameVariables.Cities[i].Population;

        }

    }

    return population;

}

int Game::getTileFoodYield (int i, int j) {

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

int Game::getTileProductionYield (int i, int j) {

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

void Game::generateCiv (int Civ) {

    std::vector<Civilization> civs;

    loadCivilizationsFromFile("civilizations.sav",civs);

     bool loopSpawn = true;

     while (loopSpawn) {

        Civilization civ;

        int x = rand() % (gameVariables.worldMap.worldSize);
        int y = rand() % (gameVariables.worldMap.worldSize*4);
        int numberOfCivilizationsPositionIsFarAwayFrom = 0;

        if (gameVariables.Civilizations.size() >= 1){

            for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++){

                if (sharedMethods::getDistance(x, y, gameVariables.Civilizations[i].startingX, gameVariables.Civilizations[i].startingY) > 5){
                    numberOfCivilizationsPositionIsFarAwayFrom++;
                }

            }
        }

        if (gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::OCEAN && gameVariables.worldMap.featureMap[x][y] != gameVariables.worldMap.mapTiles::COAST && returnLandTiles(x, y) > 2 && numberOfCivilizationsPositionIsFarAwayFrom == gameVariables.Civilizations.size()) {

                loopSpawn = false;

                std::cout << "Is Civilization #" << gameVariables.Civilizations.size() << " being played by a human? (Y/N)" << std::endl;

                char Choice;

                std::cin >> Choice;

                if (Choice == 'y' || Choice == 'Y') {

                    std::cout << "Pick a civilization to choose from." << std::endl;

                    for (int i = 0; i < civs.size(); i++) {

                        std::cout << "[" << i << "] " << civs[i].CivName << std::endl;

                    }

                    int choice = sharedMethods::bindIntegerInputToRange(0, civs.size()-1, 0);

                    civ = civs[choice];

                    civ.playedByHumans = true;

                } else {

                    int civIndex = rand() % civs.size();

                    civ = civs[civIndex];

                }

                civ.AvailableUnitsToCreate.push_back("Warrior");
                civ.AvailableUnitsToCreate.push_back("Explorer");

                civ.startingX = x; civ.startingY = y; gameVariables.Civilizations.push_back(civ);

                foundCity(x, y, gameVariables.Civilizations.size() - 1);

                loadTechnologiesFromFile("techs.sav", gameVariables.Civilizations.size() - 1);

                gameVariables.Civilizations[gameVariables.Civilizations.size() - 1].technologyBeingResearched.researchName = "";

                /** SPAWN IN CIV UNITS **/

                Unit explorer = gameVariables.Units[sharedMethods::getUnitIndexByName("Explorer", gameVariables)];
                explorer.position.setCoordinates(civ.startingX, civ.startingY);
                explorer.parentCivilizationIndex = gameVariables.Civilizations.size() - 1;

                gameVariables.UnitsInGame.push_back(explorer);

                Unit warrior = gameVariables.Units[sharedMethods::getUnitIndexByName("Warrior", gameVariables)];
                warrior.position.setCoordinates(civ.startingX, civ.startingY);
                warrior.parentCivilizationIndex = gameVariables.Civilizations.size() - 1;

                gameVariables.UnitsInGame.push_back(warrior);


        }

    }

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (sharedMethods::getDistance(gameVariables.Civilizations[Civ].startingX, gameVariables.Civilizations[Civ].startingY, i, j) <= 3) {

                gameVariables.Civilizations[Civ].WorldExplorationMap[i][j] = 1;

            } else {

                gameVariables.Civilizations[Civ].WorldExplorationMap[i][j] = 0;

            }
        }

    }

}

void Game::UpdateCivilizationExploredTerritory (int civilizationIndex) {
    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[i][j] == (civilizationIndex+1)) {

                for (int k = 0; k < gameVariables.worldMap.worldSize; k++) {

                    for (int l = 0; l < gameVariables.worldMap.worldSize*4; l++) {

                        if (sharedMethods::getDistance(i,j,k,l) < 3 && gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[k][l] == 0) { gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[k][l] = 1; }

                    }

                }

            }

        }

    }
}

void Game::DisplayCivilizationExplorationProgress (int civilizationIndex) {

    double tilesExplored = 0;

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.Civilizations[civilizationIndex].WorldExplorationMap[i][j] == 1) { tilesExplored++; }

        }

    }

    double percentage = 100 * tilesExplored / (gameVariables.worldMap.worldSize*gameVariables.worldMap.worldSize*4);

    std::cout << "\nYour civilization has explored " << percentage << "% of the world." << std::endl;
}

void Game::DisplayCitiesStatusesOwnedByCivilization (int civilizationIndex) {

    int x = 0; std::vector<int> cityIndices; double FoodNeededForNewCitizen;/*flag (r:better var name*/

    std::cout << "CITIES" << std::endl;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) {

            FoodNeededForNewCitizen = pow((gameVariables.Cities[i].Population - 1), 2.1) + 5;

            double BarsToFill = ((gameVariables.Cities[i].FoodSurplus / FoodNeededForNewCitizen) * 10);

            std::cout << x+1 << ": " << gameVariables.Cities[i].cityName << " - " << gameVariables.Cities[i].position.x << ", " << gameVariables.Cities[i].position.y << " | FOOD YIELD : " << (gameVariables.Cities[i].FoodPerTurnFromCity + gameVariables.Cities[i].FoodPerTurnFromTiles) << " (" << gameVariables.Cities[i].FoodPerTurnFromTiles <<  ") | POPULATION : " << gameVariables.Cities[i].Population << " [";

            for (int j = 0; j < (int)BarsToFill; j++) {
                std::cout << "\033[32m*";
            }
            for (int j = 0; j < (int)(10-BarsToFill); j++) {
                std::cout << "\033[0m ";
            }
            std::cout << "\e[0m] | PRODUCTION: ";

            std::cout << gameVariables.Cities[i].ProductionPerTurn + gameVariables.Cities[i].ProductionFromTiles;

            std::cout << " | GPT: " << gameVariables.Cities[i].GoldPerTurn + gameVariables.Cities[i].GoldPerTurnFromCity << std::endl;

            std::string productionString = "";

            if (gameVariables.Cities[i].isProducing == false) {

                productionString = "nothing";

            } else if (gameVariables.Cities[i].isProducingUnit == false) {

                productionString = gameVariables.Cities[i].buildingBeingProduced.name;

            } else {

                 productionString = gameVariables.Cities[i].unitBeingProduced.name;

            }

            std::cout << "  - The city is currently producing " << productionString << ".";
            x++; int y = i; cityIndices.push_back(y);

        }

    }

    char choice;

    std::cin >> choice;

    if (choice == 'p' || choice == 'P') {

        std::cout << "What city do you want to produce for?" << std::endl;

        int choice = 0;

        choice = sharedMethods::bindIntegerInputToRange (0, cityIndices.size()-1, 0);

        setCityProduction (cityIndices[choice]);

    }
}

void Game::showAvailableUnits (int civilizationIndex, bool forProduction = false, bool forBuying = true) {

    std::cout << "UNITS AVAILABLE" << std::endl;

    if (forProduction == true) {

        for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

            std::cout << "[" << i << "] : " << gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i] << " | " << gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i], gameVariables)].productionCost << std::endl;

        }

    }

    if (forBuying == true) {

        for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size(); i++) {

            std::cout << "[" << i << "] : " << gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i] << " | $" << gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[i], gameVariables)].goldCost << std::endl;

        }

    }

}

void Game::showAvailableBuildings (int cityIndex) {

    std::cout << "BUILDINGS AVAILABLE" << std::endl;

    for (unsigned int i = 0; i < gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.size(); i++) {

        std::cout << "[" << i << "] : " << gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[i] << " | " << gameVariables.Buildings[sharedMethods::getBuildingIndexByName(gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[i], gameVariables)].ProductionCost << std::endl;

    }

}

void Game::buyUnits (int civilizationIndex) {


    showAvailableUnits (civilizationIndex);

    int choice = sharedMethods::bindIntegerInputToRange(0, gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size()-1, 0);

    if (gameVariables.Civilizations[civilizationIndex].Gold >= gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[choice], gameVariables)].goldCost) {

        Unit unit = gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[choice], gameVariables)];

        unit.position.x = gameVariables.Civilizations[civilizationIndex].startingX;
        unit.position.y = gameVariables.Civilizations[civilizationIndex].startingY;
        unit.parentCivilizationIndex = gameVariables.Civilizations.size() - 1;

        gameVariables.UnitsInGame.push_back(unit);

        std::cout << "Bought a(n) " << unit.name << "!" << std::endl;

    } else {

        std::cout << "Not enough gold!" << std::endl;

    }
}

void Game::getPlayerChoiceAndReact (int civilizationIndex) {

    char Choice;

    std::cin >> Choice;

    if (Choice == characterActionCodes::EXPLORATION) {

        DisplayCivilizationExplorationProgress(civilizationIndex);

    }

    if (Choice == 'c' || Choice == 'C') {

        DisplayCitiesStatusesOwnedByCivilization(civilizationIndex);

        std::cin >> Choice;

    }

    if (Choice == 'T' || Choice == 't') {

        trade (civilizationIndex);

    }

    if (Choice == 'r' || Choice == 'R') {

        pickResearchForHumanPlayer (civilizationIndex);

    }

    if (Choice == 'o') {

        playerRequestAlliance (civilizationIndex);

    }

    if (Choice == ';') { renderer.spectate(turnNumber, gameVariables); }

    if (Choice == 's') { saveGame ("save.save"); }

    if (Choice == 'u' || Choice == 'U') {

        int x = 0; int militarypower = 0; std::vector<int> unitIndices; /*flag (r:better var name*/

        std::cout << "UNITS" << std::endl;

        for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

            if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civilizationIndex) {

                std::cout << x << ": " << gameVariables.UnitsInGame[i].name << " - " << gameVariables.UnitsInGame[i].position.x << ", " << gameVariables.UnitsInGame[i].position.y << " | MOVEMENT LEFT : " << gameVariables.UnitsInGame[i].movementPoints << std::endl;

                x++; int y = i; unitIndices.push_back(y);

                militarypower += (gameVariables.UnitsInGame[i].combat * gameVariables.UnitsInGame[i].combat);

            }

        }

        std::cout << "Your Civilization's Military Power: " << militarypower << std::endl;

        std::cin >> Choice;

        if (Choice == 'm' || Choice == 'M') {

            std::cout << "\nMove which unit? " << std::endl;

            int whichOne;

            whichOne = sharedMethods::bindIntegerInputToRange(0, unitIndices.size()-1, 0);

            while (gameVariables.UnitsInGame[unitIndices[whichOne]].movementPoints > 0) {

                std::cout << "[1] move down [2] move right [3] move up [4] move left" << std::endl;

                int d;

                std::cin >> d;

                if (d == 1 && gameVariables.UnitsInGame[unitIndices[whichOne]].position.x < gameVariables.worldMap.worldSize) { sharedMethods::moveUnit(gameVariables.UnitsInGame[unitIndices[whichOne]], gameVariables.UnitsInGame[unitIndices[whichOne]].position.x+1, gameVariables.UnitsInGame[unitIndices[whichOne]].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap); std::cout << gameVariables.UnitsInGame[unitIndices[whichOne]].movementPoints; }
                if (d == 2 && gameVariables.UnitsInGame[unitIndices[whichOne]].position.y < gameVariables.worldMap.worldSize*4) { sharedMethods::moveUnit(gameVariables.UnitsInGame[unitIndices[whichOne]], gameVariables.UnitsInGame[unitIndices[whichOne]].position.x, gameVariables.UnitsInGame[unitIndices[whichOne]].position.y+1, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap); }
                if (d == 3 && gameVariables.UnitsInGame[unitIndices[whichOne]].position.x > 0) { sharedMethods::moveUnit(gameVariables.UnitsInGame[unitIndices[whichOne]], gameVariables.UnitsInGame[unitIndices[whichOne]].position.x-1, gameVariables.UnitsInGame[unitIndices[whichOne]].position.y, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap); }
                if (d == 4 && gameVariables.UnitsInGame[unitIndices[whichOne]].position.y > 0) { sharedMethods::moveUnit(gameVariables.UnitsInGame[unitIndices[whichOne]], gameVariables.UnitsInGame[unitIndices[whichOne]].position.x, gameVariables.UnitsInGame[unitIndices[whichOne]].position.y-1, gameVariables.Civilizations[civilizationIndex], gameVariables.worldMap); }
            }
        }

        if (Choice == 'd') {

            std::cout << "\nView which unit's details? " << std::endl;

            int whichOne;

            whichOne = sharedMethods::bindIntegerInputToRange(0, unitIndices.size()-1, 0);

            displayUnitDetails (gameVariables.UnitsInGame[unitIndices[whichOne]]);
        }

        if (Choice == 't' || Choice == 'T') {

            std::cout << "\nTrain which unit? " << std::endl;

            int whichOne;

            whichOne = sharedMethods::bindIntegerInputToRange(0, unitIndices.size()-1, 0);

            gameVariables.UnitsInGame[unitIndices[whichOne]].isTraining = true;

        }

        if (Choice == 'b' || Choice == 'B') {

            buyUnits (civilizationIndex);
        }

    }

    if (Choice == 'w') {

        for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].resources.size(); i++) {

            std::cout << resourceNames[gameVariables.Civilizations[civilizationIndex].resources[i].ResourceCode] << " : " << gameVariables.Civilizations[civilizationIndex].resources[i].AmountOfResource << std::endl;

        }

    }

    if (Choice == 'e' || Choice == 'E') {

        if (gameVariables.Civilizations[civilizationIndex].technologyBeingResearched.researchName != "") {

            loopVariable = false; std::cout << "Ending your turn..." << std::endl;

        } else {

            std::cout << "You need to pick a technology to research!" << std::endl;

        }
    }

    if (Choice == 'Q') {

        std::cout << "Exiting game..." << std::endl;

        loopVariable = false;
        gameLoopVariable = false;

    }

}

void Game::assignWorkByPopulation (int cityIndex, bool stopAfterNeededAmountIsCollected) {

    int assignedCitizens = 0;

    int highestFoodValueTiles[gameVariables.Cities[cityIndex].Population];

    int tileArraySize = (sizeof(highestFoodValueTiles)/sizeof(*highestFoodValueTiles));

    int bestFoodTileXPositions[gameVariables.Cities[cityIndex].Population];

    int bestFoodTileYPositions[gameVariables.Cities[cityIndex].Population];

    for (int a = 0; a < tileArraySize; a++) { highestFoodValueTiles[a] = 0; }

    int FoodNeeded = (pow((gameVariables.Cities[cityIndex].Population - 1), 2) + 5);

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

                int yield = getTileFoodYield (i + gameVariables.Cities[cityIndex].position.x, j + gameVariables.Cities[cityIndex].position.y);

                for (int k = 0; k < tileArraySize; k++) {

                    if (yield > highestFoodValueTiles[k] && gameVariables.worldMap.WorldTerritoryMap[i + gameVariables.Cities[cityIndex].position.x][j + gameVariables.Cities[cityIndex].position.y] == gameVariables.Cities[cityIndex].parentIndex+1) {

                        highestFoodValueTiles[k] = yield;
                        bestFoodTileXPositions[k] = i + gameVariables.Cities[cityIndex].position.x;
                        bestFoodTileYPositions[k] = j + gameVariables.Cities[cityIndex].position.y;
                        gameVariables.Cities[cityIndex].FoodPerTurnFromTiles += yield;
                        gameVariables.Cities[cityIndex].ProductionFromTiles += getTileProductionYield(i + gameVariables.Cities[cityIndex].position.x, j + gameVariables.Cities[cityIndex].position.y);
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

void Game::produceUnits (int cityIndex, int civilizationIndex) {

    showAvailableUnits (civilizationIndex, true, false);

    int choice = sharedMethods::bindIntegerInputToRange(0, gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate.size()-1, 0);

    Unit unit = gameVariables.Units[sharedMethods::getUnitIndexByName(gameVariables.Civilizations[civilizationIndex].AvailableUnitsToCreate[choice], gameVariables)];

    gameVariables.Cities[cityIndex].unitBeingProduced = unit;

    gameVariables.Cities[cityIndex].isProducing = true;
    gameVariables.Cities[cityIndex].isProducingUnit = true;

}

void Game::produceBuilding (int cityIndex, int civilizationIndex) {

    showAvailableBuildings (civilizationIndex);

    int choice = sharedMethods::bindIntegerInputToRange(0, gameVariables.Cities[cityIndex].AvailableBuildingsToCreate.size()-1, 0);

    Building building = gameVariables.Buildings[sharedMethods::getBuildingIndexByName(gameVariables.Cities[cityIndex].AvailableBuildingsToCreate[choice], gameVariables)];

    gameVariables.Cities[cityIndex].buildingBeingProduced = building;

    gameVariables.Cities[cityIndex].isProducing = true;
    gameVariables.Cities[cityIndex].isProducingUnit = false;

}

void Game::setCityProduction (int cityIndex) {

    std::cout << "What do you want to produce in the city of " << gameVariables.Cities[cityIndex].cityName << "?\n[1] Units\n[2] Buildings" << std::endl;

    int choice = sharedMethods::bindIntegerInputToRange(1,2,1);

    if (choice == 1) {

        produceUnits (cityIndex, gameVariables.Cities[cityIndex].parentIndex);

    }

    if (choice == 2) {

        produceBuilding (cityIndex, gameVariables.Cities[cityIndex].parentIndex);

    }
}

void Game::updateCityProductionModifier (int cityIndex, int civilizationIndex) {

    double tempModifier = 100;

    if (gameVariables.Civilizations[civilizationIndex].Happiness >= 70) {

        double happinessModifier = (gameVariables.Civilizations[civilizationIndex].Happiness-70);

        tempModifier += happinessModifier;

    } else if (gameVariables.Civilizations[civilizationIndex].Happiness <= 25) {

        tempModifier -= 25;

    }

    if (civilizationHasTechnology(civilizationIndex, "Masonry")) {

        tempModifier += 5;

    }

    gameVariables.Cities[cityIndex].ProductionModifier = (tempModifier/100);

}

void Game::updateCityUnitProduction (int cityIndex, int civilizationIndex) {

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

void Game::updateCityBuildingProduction (int cityIndex, int civilizationIndex) {

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

int Game::getTerrainCodeUnitIsOn (Unit &Unit) {

    return gameVariables.worldMap.featureMap[Unit.position.x][Unit.position.y];

}

void Game::updateUnitTraining () {

    for (int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].isTraining) {

            switch (getTerrainCodeUnitIsOn(gameVariables.UnitsInGame[i])) {

                case 2: // grassland
                    gameVariables.UnitsInGame[i].grasslandModifier.experience += 0.1;
                break;
                case 3: // mountain
                    gameVariables.UnitsInGame[i].mountainModifier.experience += 0.1;
                break;
                case 4: // forest
                    gameVariables.UnitsInGame[i].forestModifier.experience += 0.1;
                break;
                case 6: // snow
                    gameVariables.UnitsInGame[i].snowModifier.experience += 0.1;
                break;
                case 7: // desert
                    gameVariables.UnitsInGame[i].desertModifier.experience += 0.1;
                break;

            }

        }

    }

}

bool Game::isTileBorderingCivilizationTerritory (int x, int y, int civilizationIndex) {

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

bool Game::canCityExpand (int cityIndex, int civilizationIndex) {

    for (int i = (-1*cityExpansionLimit); i < cityExpansionLimit+1; i++) {

        for (int j = (-1*cityExpansionLimit); j < cityExpansionLimit+1; j++) {

            if (gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] == 0
            && isTileBorderingCivilizationTerritory(gameVariables.Cities[cityIndex].position.x+i,
            gameVariables.Cities[cityIndex].position.y+j,
            civilizationIndex)) {

                return true;

            }

        }

    }
    std::cout << "false";
    return false;

}

void Game::expandCityTerritory (int cityIndex) {

    if (canCityExpand(cityIndex, gameVariables.Cities[cityIndex].parentIndex)) {

        bool tileFound = false;

        while (!tileFound) {

            int i = rand() % (cityExpansionLimit * 2) - cityExpansionLimit;
            int j = rand() % (cityExpansionLimit * 2) - cityExpansionLimit;

            if (gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] == 0
                && isTileBorderingCivilizationTerritory(gameVariables.Cities[cityIndex].position.x+i,
                gameVariables.Cities[cityIndex].position.y+j,
                gameVariables.Cities[cityIndex].parentIndex)) {

                gameVariables.worldMap.WorldTerritoryMap[gameVariables.Cities[cityIndex].position.x + i][gameVariables.Cities[cityIndex].position.y + j] = gameVariables.Cities[cityIndex].parentIndex + 1;

                tileFound = true;

            }

        }

    }

    gameVariables.Cities[cityIndex].turnsToExpand = 5;

}

void Game::requestAlliance (int civilizationIndex, int targetCivilizationIndex) {

    Event alliance;

    alliance.eventType = "alliance";

    alliance.test_canBeTriggered = true;

    alliance.EventName = "Alliance Request from " + gameVariables.Civilizations[civilizationIndex].CivName;

    alliance.EventMessage = "The wise leaders of " + gameVariables.Civilizations[civilizationIndex].CivName + " recognize that an alliance with us would be beneficial and request a formal alliance.";

    alliance.ResponseChoices.push_back("Accept Request");

    alliance.ResponseChoices.push_back("Deny Request");

    alliance.initializerCivilization = civilizationIndex; alliance.targetCivilizationIndex = targetCivilizationIndex;

    gameEvents.push_back(alliance);

}

void Game::playerRequestAlliance (int civilizationIndex) {

    std::cout << "Request an alliance with what Civilization?" << std::endl;

    for (unsigned int a = 0; a < gameVariables.Civilizations.size(); a++) {

        if (a != civilizationIndex) {

            std::cout << "[" << a << "] " << gameVariables.Civilizations[a].CivName << std::endl;

        }
    }

    int Choice = sharedMethods::bindIntegerInputToRange(0,gameVariables.Civilizations.size()-1,0);

    requestAlliance (civilizationIndex, Choice);

}

void Game::updateEvents (int civilizationIndex) {

    for (unsigned int e = 0; e < gameEvents.size(); e++) {

        gameEvents[e].listen(civilizationIndex, gameVariables);

        if (gameEvents[e].hasBeenTriggered == true) {

            gameEvents.erase(gameEvents.begin() + e);

        }

    }

}

void Game::updateCivilizationHappiness (int civilizationIndex) {

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

void Game::updateResources (int civilizationIndex) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++) {

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++) {

            if (gameVariables.worldMap.WorldResourceMap[i][j].ResourceCode != 0 && gameVariables.worldMap.WorldTerritoryMap[i][j] == civilizationIndex+1) {

                gameVariables.Civilizations[civilizationIndex].resources.push_back(gameVariables.worldMap.WorldResourceMap[i][j]);

            }

        }

    }

}

void Game::updateCities () {

    for (int w = 0; w < gameVariables.Cities.size(); w++) {

        gameVariables.Cities[w].FoodSurplus += ((gameVariables.Cities[w].FoodPerTurnFromCity + gameVariables.Cities[w].FoodPerTurnFromTiles) - (gameVariables.Cities[w].Population*2));

        gameVariables.Cities[w].turnsToExpand -= gameVariables.Civilizations[gameVariables.Cities[w].parentIndex].ExpansionRate;

        if (gameVariables.Cities[w].turnsToExpand <= 0) {

            expandCityTerritory(w);

        }

        if (gameVariables.Cities[w].FoodSurplus >= (pow((gameVariables.Cities[w].Population - 1), 2) + 5)) {
            gameVariables.Cities[w].FoodSurplus -= pow((gameVariables.Cities[w].Population - 1), 2) + 5;
            gameVariables.Cities[w].Population++;
            gameVariables.Civilizations[gameVariables.Cities[w].parentIndex].Happiness -= 2;
            assignWorkByPopulation(w, false);

        }

        if (gameVariables.Cities[w].FoodSurplus < 0) {
            gameVariables.Cities[w].Population--;
            assignWorkByPopulation(w, false);

        }

        updateCityProductionModifier (w, gameVariables.Cities[w].parentIndex);

        if (gameVariables.Cities[w].isProducingUnit == true) {

            updateCityUnitProduction (w, gameVariables.Cities[w].parentIndex);

        } else {

            updateCityBuildingProduction (w, gameVariables.Cities[w].parentIndex);

        }

    }

}

void Game::displayCivilizationResources (int civilizationIndex) {

    for (int i = 0; i < gameVariables.Civilizations[civilizationIndex].resources.size(); i++) {

        std::cout << "[" << i << "] " << resourceNames[gameVariables.Civilizations[civilizationIndex].resources[i].ResourceCode] << " x" << gameVariables.Civilizations[civilizationIndex].resources[i].AmountOfResource << std::endl;

    }

}

void Game::displayUnitDetails (Unit unit) {

    std::cout << unit.name << " - " << unit.position.x << ", " << unit.position.y << std::endl;
    std::cout << "HEALTH: " << unit.health << " / " << unit.maxHealth << std::endl;
    std::cout << "TERRAIN MODIFIERS\n~ Grassland\n  ~ Attack " << unit.grasslandModifier.attackModifier*100 << " | Defense " << unit.grasslandModifier.defenseModifier*100 << std::endl;
    std::cout << "~ Forest\n  ~ Attack " << unit.forestModifier.attackModifier*100 << " | Defense " << unit.forestModifier.defenseModifier*100 << std::endl;
    std::cout << "~ Mountain\n  ~ Attack " << unit.mountainModifier.attackModifier*100 << " | Defense " << unit.mountainModifier.defenseModifier*100 << std::endl;
    std::cout << "~ Desert\n  ~ Attack " << unit.desertModifier.attackModifier*100 << " | Defense " << unit.desertModifier.defenseModifier*100 << std::endl;
    std::cout << "~ Snow\n  ~ Attack " << unit.snowModifier.attackModifier*100 << " | Defense " << unit.snowModifier.defenseModifier*100 << std::endl;

    std::cin.ignore();
}

int Game::inputResourcesToTrade (int civilizationIndex, Trade &trade) {

    displayCivilizationResources (civilizationIndex);

    int selectedResourceIndex = 0;

    selectedResourceIndex = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations[civilizationIndex].resources.size()-1, 0);

}

bool Game::resourceIsNotAlreadyBeingTraded (int civilizationIndex, int resourceIndex, std::vector<Resource> resources) {

    for (unsigned int i = 0; i < resources.size(); i++) {

        if (gameVariables.Civilizations[civilizationIndex].resources[resourceIndex].ResourceCode == resources[i].ResourceCode) {

            return false;

        }

    }

    return true;

}

void Game::editTrade (int civilizationIndex, Trade &trade, int Choice) {

    bool isStillEditingTrade = true;

    while (isStillEditingTrade) {

        std::cout << "= YOUR ITEMS =" << std::endl;

        std::cout << "GOLD: " << trade.goldSumFromTrader << "\nGPT: " << trade.GPTFromTrader << std::endl;

        std::cout << "RESOURCES: " << std::endl;

        for (int i = 0; i < trade.resourcesFromTrader.size(); i++) {

            std::cout << resourceNames[trade.resourcesFromTrader[i].ResourceCode] << std::endl;

        }

        std::cout << "= THEIR ITEMS =" << std::endl;

        std::cout << "GOLD: " << trade.goldSumFromRecipient << "\nGPT: " << trade.GPTFromRecipient << std::endl;

        std::cout << "RESOURCES: " << std::endl;

        for (int i = 0; i < trade.resourcesFromRecipient.size(); i++) {

            std::cout << resourceNames[trade.resourcesFromRecipient[i].ResourceCode] << std::endl;

        }

        std::cout << "\n[Z] Edit your gold sum [X] Edit their gold sum [C] Edit your resources to trade [V] Edit their resources to trade\n[B] Edit your GPT [N] Edit their GPT [E] Confirm trade" << std::endl;

        char characterInput;

        std::cin >> characterInput;

        if (characterInput == 'E' || characterInput == 'e') {

            isStillEditingTrade = false;

        }

        if (characterInput == 'Z' || characterInput == 'z') {

            int amountOfGold = 0;

            std::cout << "Please input the amount of gold you want to trade: ";

            amountOfGold = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations[civilizationIndex].Gold, 0);

            trade.goldSumFromTrader = amountOfGold;

        }

        if (characterInput == 'X' || characterInput == 'x') {

            int amountOfGold = 0;

            std::cout << "Please input the amount of gold you want them to trade: ";

            amountOfGold = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations[Choice].Gold, 0);

            trade.goldSumFromRecipient = amountOfGold;

        }

        if (characterInput == 'C' || characterInput == 'c') {

            std::cout << "[R]eset Traded Resources or [A]dd New Resources" << std::endl;

            std::cin >> characterInput;

            if (characterInput == 'R' || characterInput == 'r') {

                trade.resourcesFromTrader.clear();

            }

            if (characterInput == 'A' || characterInput == 'a') {


                int index = inputResourcesToTrade (civilizationIndex, trade);

                if (gameVariables.Civilizations[civilizationIndex].resources.size() > 0 && resourceIsNotAlreadyBeingTraded(civilizationIndex, index, trade.resourcesFromTrader)) {

                    trade.resourcesFromTrader.push_back (gameVariables.Civilizations[civilizationIndex].resources[index]);

                }

            }

        }

        if (characterInput == 'V' || characterInput == 'v') {

            std::cout << "[R]eset Traded Resources or [A]dd New Resources" << std::endl;

            std::cin >> characterInput;

            if (characterInput == 'R' || characterInput == 'r') {

                trade.resourcesFromRecipient.clear();

            }

            if (characterInput == 'A' || characterInput == 'a') {


                int index = inputResourcesToTrade (Choice, trade);

                if (gameVariables.Civilizations[Choice].resources.size() > 0 && resourceIsNotAlreadyBeingTraded(Choice, index, trade.resourcesFromRecipient)) {

                    trade.resourcesFromRecipient.push_back (gameVariables.Civilizations[Choice].resources[index]);

                }

            }

        }

        if (characterInput == 'B' || characterInput == 'b') {

            int amountOfGoldPerTurn = 0;

            std::cout << "Please input the amount of gold per turn you want to trade: ";

            amountOfGoldPerTurn = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations[civilizationIndex].GoldPerTurn, 0);

            trade.GPTFromTrader = amountOfGoldPerTurn;

        }

        if (characterInput == 'N' || characterInput == 'n') {

            int amountOfGoldPerTurn = 0;

            std::cout << "Please input the amount of gold per turn you want them to trade. (They have " << gameVariables.Civilizations[Choice].GoldPerTurn << "): ";

            amountOfGoldPerTurn = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations[Choice].GoldPerTurn, 0);

            trade.GPTFromRecipient = amountOfGoldPerTurn;

        }

    }

}

void Game::trade (int civilizationIndex) {

    int Choice = 0;

    std::cout << "Trade with what Civilization?" << std::endl;

    for (unsigned int i = 0; i < gameVariables.Civilizations.size(); i++) {

        if (i != civilizationIndex) {

            std::cout << "[" << i << "] " << gameVariables.Civilizations[i].CivName << std::endl;

        }

    }

    Choice = sharedMethods::bindIntegerInputToRange (0, gameVariables.Civilizations.size() -1, 0);

    Trade trade;

    trade.recipientIndex = Choice;
    trade.traderIndex = civilizationIndex;

    editTrade (civilizationIndex, trade, Choice);

    trades.push_back (trade);

}

void Game::updateTrades (int civilizationIndex) {

    for (unsigned int i = 0; i < trades.size(); i++) {

        if (trades[i].recipientIndex == civilizationIndex && gameVariables.Civilizations[civilizationIndex].playedByHumans == true) {

            std::cout << gameVariables.Civilizations[trades[i].traderIndex].CivName << " has requested a trade. " << std::endl;

            std::cout << "= YOUR ITEMS =" << std::endl;

            std::cout << "GOLD: " << trades[i].goldSumFromRecipient << "\nGPT: " << trades[i].GPTFromRecipient << std::endl;

            std::cout << "RESOURCES: " << std::endl;

            for (int j = 0; j < trades[i].resourcesFromRecipient.size(); j++) {

                std::cout << resourceNames[trades[i].resourcesFromRecipient[j].ResourceCode] << std::endl;

            }

            std::cout << "= THEIR ITEMS =" << std::endl;

            std::cout << "GOLD: " << trades[i].goldSumFromTrader << "\nGPT: " << trades[i].GPTFromTrader << std::endl;

            std::cout << "RESOURCES: " << std::endl;

            for (int j = 0; j < trades[i].resourcesFromTrader.size(); j++) {

                std::cout << resourceNames[trades[i].resourcesFromTrader[j].ResourceCode] << std::endl;

            }

            std::cout << "[A]CCEPT OR [D]ENY?" << std::endl;

            char Choice;

            std::cin >> Choice;

            if (Choice == 'a' || Choice == 'A') {

                Event tradeAccepted;

                tradeAccepted.EventName = "Trade Accepted";

                tradeAccepted.EventMessage = "Your trade request has been accepted.";

                tradeAccepted.targetCivilizationIndex = trades[i].traderIndex;

                gameEvents.push_back (tradeAccepted);


                gameVariables.Civilizations[trades[i].traderIndex].Gold -= trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold += trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold -= trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].Gold += trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].GoldPerTurn -= trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn += trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn -= trades[i].GPTFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].GoldPerTurn += trades[i].GPTFromRecipient;


                for (unsigned int k = 0; k < trades[i].resourcesFromTrader.size(); k++) {

                    gameVariables.Civilizations[civilizationIndex].resources.push_back (trades[i].resourcesFromTrader[k]);

                    gameVariables.Civilizations[trades[i].traderIndex].resources.erase (gameVariables.Civilizations[trades[i].traderIndex].resources.begin() + k);


                }

                for (unsigned int k = 0; k < trades[i].resourcesFromRecipient.size(); k++) {

                    gameVariables.Civilizations[trades[i].traderIndex].resources.push_back (trades[i].resourcesFromRecipient[k]);

                    gameVariables.Civilizations[civilizationIndex].resources.erase (gameVariables.Civilizations[civilizationIndex].resources.begin() + k);


                }

            } else {

                Event tradeDenied;

                tradeDenied.EventName = "Trade Denied";

                tradeDenied.EventMessage = "Your trade request has been denied.";

                tradeDenied.targetCivilizationIndex = trades[i].traderIndex;

                gameEvents.push_back (tradeDenied);


            }

            trades.erase (trades.begin() + i);

        }

        if (trades[i].recipientIndex == civilizationIndex && gameVariables.Civilizations[civilizationIndex].playedByHumans == false) {

            if (ai.returnTradeValue (trades[i], gameVariables) >= 0) {

                Event tradeAccepted;

                tradeAccepted.EventName = "Trade Accepted";

                tradeAccepted.EventMessage = "Your trade request has been accepted.";

                tradeAccepted.targetCivilizationIndex = trades[i].traderIndex;

                gameEvents.push_back (tradeAccepted);

                gameVariables.Civilizations[trades[i].traderIndex].Gold -= trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold += trades[i].goldSumFromTrader;

                gameVariables.Civilizations[civilizationIndex].Gold -= trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].Gold += trades[i].goldSumFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].GoldPerTurn -= trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn += trades[i].GPTFromTrader;

                gameVariables.Civilizations[civilizationIndex].GoldPerTurn -= trades[i].GPTFromRecipient;

                gameVariables.Civilizations[trades[i].traderIndex].GoldPerTurn += trades[i].GPTFromRecipient;

                for (unsigned int k = 0; k < trades[i].resourcesFromTrader.size(); k++) {

                    gameVariables.Civilizations[civilizationIndex].resources.push_back (trades[i].resourcesFromTrader[k]);

                    gameVariables.Civilizations[trades[i].traderIndex].resources.erase (gameVariables.Civilizations[trades[i].traderIndex].resources.begin() + k);


                }

                for (unsigned int k = 0; k < trades[i].resourcesFromRecipient.size(); k++) {

                    gameVariables.Civilizations[trades[i].traderIndex].resources.push_back (trades[i].resourcesFromRecipient[k]);

                    gameVariables.Civilizations[civilizationIndex].resources.erase (gameVariables.Civilizations[civilizationIndex].resources.begin() + k);


                }

            } else {

                Event tradeDenied;

                tradeDenied.EventName = "Trade Denied";

                tradeDenied.EventMessage = "Your trade request has been denied.";

                tradeDenied.targetCivilizationIndex = trades[i].traderIndex;

                gameEvents.push_back (tradeDenied);

            }

            trades.erase (trades.begin() + i);

        }

    }

}

void Game::updateGoldPerTurn (int civilizationIndex) {

    int gpt = 0;

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].parentIndex == civilizationIndex) {

            gpt += gameVariables.Cities[i].GoldPerTurn;
            gpt += gameVariables.Cities[i].GoldPerTurnFromCity;

        }

    }

    gameVariables.Civilizations[civilizationIndex].GoldPerTurn = gpt;

}

void Game::loop () {

    gameLoopVariable = true;

    for (int civilizationIndex = 0; civilizationIndex < gameVariables.Civilizations.size(); civilizationIndex++) {

        updateResources (civilizationIndex);

    }

    while (gameLoopVariable == true){

        renderer.worldMap = gameVariables.worldMap;

        updateUnitTraining ();

        for (int civilizationIndex = 0; civilizationIndex < gameVariables.Civilizations.size(); civilizationIndex++) {

            loopVariable = true;

            updateEvents (civilizationIndex);

            updateResources (civilizationIndex);

            updateCivilizationHappiness (civilizationIndex);

            updateTrades (civilizationIndex);

            updateGoldPerTurn (civilizationIndex);

            gameVariables.Civilizations[civilizationIndex].Gold += gameVariables.Civilizations[civilizationIndex].GoldPerTurn;

            if (gameVariables.Civilizations[civilizationIndex].playedByHumans) {

                while (loopVariable == true) {
                    renderer.render(civilizationIndex, turnNumber, gameVariables);

                    UpdateCivilizationExploredTerritory(civilizationIndex);

                    getPlayerChoiceAndReact(civilizationIndex);

                }

            } else {

                ai.think(civilizationIndex, gameVariables, trades);
                ai.moveAllUnitsBelongingToCiv(civilizationIndex, gameVariables);

            }

            gameVariables.Civilizations[civilizationIndex].researchPoints += (Game::getCivilizationPopulation(civilizationIndex) * gameVariables.Civilizations[civilizationIndex].ScienceRate);

            updateResearch (civilizationIndex);

        }

        turnNumber++;

        updateCities ();

        for (unsigned int a = 0; a < gameVariables.UnitsInGame.size(); a++) {

            if (gameVariables.UnitsInGame[a].isTraining == false) {

            gameVariables.UnitsInGame[a].movementPoints = gameVariables.UnitsInGame[a].maxMovePoints;

            }

        }
    }

}
