#include "sharedMethods.h"
#include <math.h>
#include <iostream>
#include <thread>
#include <climits>


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

int getUnitIndexByName (std::string name, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Units.size(); i++) {

        if (gameVariables.Units[i].name == name) {

            return i;

        }

    }

    std::cout << "Couldn't find unit!";

    return 0;

}

void moveUnit (Unit &unit, int xPositionToMoveTo, int yPositionToMoveTo, Civilization &civ, WorldMap worldMap) {

    bool isFlatTerrain = true;

    if (((unit.canCoastalEmbark == false && worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] != worldMap.mapTiles::COAST) || unit.canCoastalEmbark) && worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] != worldMap.mapTiles::OCEAN) {
        if (worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] == worldMap.mapTiles::MOUNTAIN || worldMap.featureMap[xPositionToMoveTo][yPositionToMoveTo] == worldMap.mapTiles::FOREST) {
            isFlatTerrain = false;
        }

        if (isFlatTerrain) { unit.movementPoints--; } else { unit.movementPoints -= unit.terrainMoveCost; }

        unit.position.setCoordinates(xPositionToMoveTo, yPositionToMoveTo);
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

int getResearchIndexByName (int g, std::string techName, GameVariables &game_variables) {

    for (unsigned int i = 0; i < game_variables.Civilizations[g].technologiesToResearch.size(); i++) {

        if (techName == game_variables.Civilizations[g].technologiesToResearch[i].researchName) {

            return i;

        }

    }

    return -1;

}

bool CivilizationHasPrerequisiteTechs (int g, std::string techName, GameVariables &game_variables) {

    int techPrequisitesMet = 0;

    int researchIndex = getResearchIndexByName (g, techName, game_variables);

    if (researchIndex > -1) {

        for (unsigned int i = 0; i < game_variables.Civilizations[g].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired.size(); i++) {

            for (unsigned int j = 0; j < game_variables.Civilizations[g].learnedTechnologies.size(); j++) {

                if (game_variables.Civilizations[g].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired[i] == game_variables.Civilizations[g].learnedTechnologies[j]) {

                    techPrequisitesMet++;

                }

            }

        }

    }

    if (techPrequisitesMet == game_variables.Civilizations[g].technologiesToResearch[researchIndex].prerequisiteTechnologiesRequired.size()) {
        return true;
    } else {
        return false;
    }

}

}
