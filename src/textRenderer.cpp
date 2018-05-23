#include "textRenderer.h"
#include "sharedMethods.h"
#include <iostream>
#include <sstream>

textRenderer::textRenderer()
{
    //ctor
}

textRenderer::~textRenderer()
{
    //dtor
}

bool textRenderer::isUnitAtPosition (int x, int y, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].position.x == x && gameVariables.UnitsInGame[i].position.y == y) {

            return true;

        }

    }

    return false;

}

bool textRenderer::isCityAtPosition (int x, int y, GameVariables &gameVariables) {

    for (unsigned int i = 0; i < gameVariables.Cities.size(); i++) {

        if (gameVariables.Cities[i].position.x == x && gameVariables.Cities[i].position.y == y) {

            return true;

        }

    }

    return false;

}

int textRenderer::returnCivMilitaryPower (int civ_index, GameVariables &gameVariables) {

    int militarypower = 0;

    for (unsigned int i = 0; i < gameVariables.UnitsInGame.size(); i++) {

        if (gameVariables.UnitsInGame[i].parentCivilizationIndex == civ_index) {

                militarypower += (gameVariables.UnitsInGame[i].combatStrength * gameVariables.UnitsInGame[i].combatStrength);

        }

    }

    return militarypower;

}


std::string textRenderer::colorTextByTerritory (int i, int j, GameVariables &gameVariables) {

    if (gameVariables.worldMap.WorldTerritoryMap[i][j] > 0) {

        int r = gameVariables.Civilizations[gameVariables.worldMap.WorldTerritoryMap[i][j]-1].rgbValues[0];
        int g = gameVariables.Civilizations[gameVariables.worldMap.WorldTerritoryMap[i][j]-1].rgbValues[1];
        int b = gameVariables.Civilizations[gameVariables.worldMap.WorldTerritoryMap[i][j]-1].rgbValues[2];

        std::stringstream rgbTextValue;

        rgbTextValue << "\033[48;2;" << r << ";" << g << ";" << b << "m";

        return rgbTextValue.str();

    } else {

        return "\033[0m";

    }
}

std::string textRenderer::colorTextByCity (int i, int j, GameVariables &gameVariables) {
    if (isCityAtPosition(i,j, gameVariables)) {
        return "\033[0;40m";
    } else {
        return "";
    }
}

std::string textRenderer::renderTextBasedOnWorldFeature (int i, int j, GameVariables &gameVariables) {

    std::string colorCode = "";

    if (gameVariables.worldMap.WorldResourceMap[i][j].ResourceCode != gameVariables.worldMap.resources::NONE) {
        colorCode = "\e[33m";
    }

    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::GRASSLAND){
        return colorCode + "#\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::COAST){
        return colorCode + "-\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::MOUNTAIN){
        return colorCode + "^\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::FOREST){
        return colorCode + "*\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::OCEAN) {
        return colorCode + " \e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::RUINS) {
        return colorCode + "!\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::DESERT) {
        return colorCode + ".\e[0m";
    }
    if (gameVariables.worldMap.featureMap[i][j] == gameVariables.worldMap.mapTiles::SNOW) {
        return colorCode + "s\e[0m";
    }
    return "";
}

void textRenderer::DisplayUnit (int i, int j) {

    std::cout << "@\e[0m";

}

void textRenderer::DisplayTextBasedOnFeature (int i, int j, GameVariables &gameVariables) {

        if (isUnitAtPosition(i, j, gameVariables) == false && isCityAtPosition(i, j, gameVariables) == false) {

            std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j, gameVariables) << renderTextBasedOnWorldFeature(i,j,gameVariables);

        } else if (isCityAtPosition(i,j, gameVariables) == true && isUnitAtPosition(i,j, gameVariables) == false) {

            std::cout << colorTextByTerritory(i,j,gameVariables) << "✪";

        } else if (isUnitAtPosition(i,j, gameVariables) == true) {

            std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j, gameVariables);
            DisplayUnit (i,j);

        }

}

void textRenderer::DisplayTextBasedOnEntitiesAtPosition (int i, int j, int currentCivilizationIndex, GameVariables &gameVariables) {

    if (gameVariables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 1) {

        DisplayTextBasedOnFeature (i, j, gameVariables);

    } else {

        if (i % 2 == 0 && j % 2 == 0) { std::cout << "\e[47;30m?\e[0m"; } else { std::cout << "\e[47m \e[0m"; }

    }

}

void textRenderer::renderStatusTextRightOfWorldMap (int i, int currentCivilizationIndex, int turnNumber, GameVariables &gameVariables) {

    std::cout << " |!|";

    if (i == 0) { std::cout << " - CIVILIZATIONS - "; }

    for (unsigned int w = 0; w < gameVariables.Civilizations.size(); w++) {

        if (i == w+1 && i == currentCivilizationIndex+1) {
            std::cout << " > YOU < ";
        } else if (i == w+1) {
            std::cout << " " << gameVariables.Civilizations[w].CivName << " - [" << gameVariables.Civilizations[currentCivilizationIndex].relationsWithOtherCivilizations[w] << " | " << gameVariables.Civilizations[w].relationsWithOtherCivilizations[currentCivilizationIndex] << "]";
        }

    }

    if (i == (gameVariables.Civilizations.size() + 2)) { std::cout << " - TURN " << turnNumber << " - "; }

    if (i == (gameVariables.Civilizations.size() + 5)) { std::cout << " - MILITARY POWER - "; }

    if (i == (gameVariables.Civilizations.size() + 6)) { std::cout << " " << returnCivMilitaryPower(currentCivilizationIndex, gameVariables);}

    if (i == (gameVariables.Civilizations.size() + 7)) { std::cout << " - ECONOMICS -"; }

    if (i == (gameVariables.Civilizations.size() + 8)) { std::cout << " GOLD : " << gameVariables.Civilizations[currentCivilizationIndex].Gold; }

    if (i == (gameVariables.Civilizations.size() + 9)) { std::cout << " GPT : " << gameVariables.Civilizations[currentCivilizationIndex].GoldPerTurn; }

    if (i == (gameVariables.Civilizations.size() + 10)) { std::cout << " - RESEARCH -"; }

    if (i == (gameVariables.Civilizations.size() + 11)) { std::cout << " " << gameVariables.Civilizations[currentCivilizationIndex].technologyBeingResearched.researchName; }

    if (i == (gameVariables.Civilizations.size() + 12) && gameVariables.Civilizations[currentCivilizationIndex].technologyBeingResearched.researchName != "") { std::cout << " " << gameVariables.Civilizations[currentCivilizationIndex].researchPoints << " / " << gameVariables.Civilizations[currentCivilizationIndex].technologyBeingResearched.scienceCostToLearnResearch; }

    if (i == (gameVariables.Civilizations.size() + 13)) { std::cout << " HAPPINESS : " << gameVariables.Civilizations[currentCivilizationIndex].Happiness; }

    if (i == (gameVariables.Civilizations.size() + 14)) { std::cout << " WAR SUPPORT : " << gameVariables.Civilizations[currentCivilizationIndex].warSupportPercentage << "%"; }

    std::cout << "\n";

}

void textRenderer::render (int currentCivilizationIndex, int turnNumber, GameVariables &gameVariables) {

    for (int i = 0; i < gameVariables.worldMap.worldSize; i++){

        for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++){

            DisplayTextBasedOnEntitiesAtPosition (i, j, currentCivilizationIndex, gameVariables);

        }

        renderStatusTextRightOfWorldMap (i, currentCivilizationIndex, turnNumber, gameVariables);

    }

}

void textRenderer::spectate (int turnNumber, GameVariables &gameVariables) {

    bool stillSpectating = true;

    int currentCivIndex = 0;

    while (stillSpectating) {

        if (currentCivIndex > -1) {

            render (currentCivIndex, turnNumber, gameVariables);

        } else {

            for (int i = 0; i < gameVariables.worldMap.worldSize; i++){

                for (int j = 0; j < gameVariables.worldMap.worldSize*4; j++){

                        DisplayTextBasedOnFeature (i, j, gameVariables);

                }

                std::cout << "\n";

            }

        }

        char input = ' ';

        std::cin >> input;

        if (input == 'x') {

            stillSpectating = false;

        }

        if (input == '>') {

            if (currentCivIndex < (gameVariables.Civilizations.size()-1) || currentCivIndex == -1) {

                currentCivIndex++;

            }

            else {

                currentCivIndex = -1;

            }

        }

        if (input == '<') {

            if (currentCivIndex > -1) {

                currentCivIndex--;

            } else {

                currentCivIndex = gameVariables.Civilizations.size()-1;

            }

        }

    }

}

void textRenderer::DisplayUnitGroupings (int civilizationIndex, GameVariables &gameVariables) {

    std::cout << "UNIT GROUPS" << std::endl;

    for (unsigned int i = 0; i < gameVariables.Civilizations[civilizationIndex].unitGroups.size(); i++) {

        std::stringstream rgbTextValue;

        rgbTextValue << "\033[48;2;" << gameVariables.Civilizations[civilizationIndex].unitGroups[i].rgbValue[0]
            << ";" << gameVariables.Civilizations[civilizationIndex].unitGroups[i].rgbValue[1]
            << ";" << gameVariables.Civilizations[civilizationIndex].unitGroups[i].rgbValue[2] << "m";

        std::cout << rgbTextValue.str() << " \033[0m " << gameVariables.Civilizations[civilizationIndex].unitGroups[i].name << " - "
            << gameVariables.Civilizations[civilizationIndex].unitGroups[i].memberUnitIndices.size() << std::endl;

        for (unsigned int j = 0; j < gameVariables.Civilizations[civilizationIndex].unitGroups[i].memberUnitIndices.size(); j++) {

            int unitIndex = gameVariables.Civilizations[civilizationIndex].unitGroups[i].memberUnitIndices[j];
            std::cout << " - #" << j+1 << ": " << gameVariables.UnitsInGame[unitIndex].name << std::endl;

        }

    }

}
