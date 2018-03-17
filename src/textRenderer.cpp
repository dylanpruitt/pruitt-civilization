#include "textRenderer.h"
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

            if (gameVariables.Civilizations[civ_index].LeaderTrait != "Brave") {
                militarypower += (gameVariables.UnitsInGame[i].combat * gameVariables.UnitsInGame[i].combat);
            } else {
                militarypower += ((gameVariables.UnitsInGame[i].combat*1.15) * (gameVariables.UnitsInGame[i].combat*1.15));
            }

        }

    }

    return militarypower;

}


std::string textRenderer::colorTextByTerritory (int i, int j, GameVariables &gameVariables) {

    if (worldMap.WorldTerritoryMap[i][j] > 0) {

        int r = gameVariables.Civilizations[worldMap.WorldTerritoryMap[i][j]-1].rgbValues[0];
        int g = gameVariables.Civilizations[worldMap.WorldTerritoryMap[i][j]-1].rgbValues[1];
        int b = gameVariables.Civilizations[worldMap.WorldTerritoryMap[i][j]-1].rgbValues[2];

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

std::string textRenderer::renderTextBasedOnWorldFeature (int i, int j) {

    std::string colorCode = "";

    if (worldMap.WorldResourceMap[i][j].ResourceCode != worldMap.resources::NONE) {
        colorCode = "\e[33m";
    }

    if (worldMap.featureMap[i][j] == worldMap.mapTiles::GRASSLAND){
        return colorCode + "#\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::COAST){
        return colorCode + "-\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::MOUNTAIN){
        return colorCode + "^\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::FOREST){
        return colorCode + "*\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::OCEAN) {
        return colorCode + " \e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::RUINS) {
        return colorCode + "!\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::DESERT) {
        return colorCode + ".\e[0m";
    }
    if (worldMap.featureMap[i][j] == worldMap.mapTiles::SNOW) {
        return colorCode + "s\e[0m";
    }
    return "";
}

void textRenderer::DisplayUnit (int i, int j) {

    std::cout << "@\e[0m";

}

void textRenderer::DisplayTextBasedOnEntitiesAtPosition (int i, int j, int currentCivilizationIndex, GameVariables &gameVariables) {

    if (gameVariables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 1 && isUnitAtPosition(i, j, gameVariables) == false && isCityAtPosition(i, j,gameVariables) == false) {

        std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j, gameVariables) << renderTextBasedOnWorldFeature(i,j); /*flag (r:should it just cout in the function?*/


    } else if (gameVariables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 0) {

        if(i % 2 == 0 && j % 2 == 0) { std::cout << "\e[47;30m?\e[0m"; } else { std::cout << "\e[47m \e[0m"; }

    } else if (isUnitAtPosition(i,j, gameVariables)){

        std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j, gameVariables);
        DisplayUnit (i,j);

    } else if (isCityAtPosition(i,j, gameVariables) == true && gameVariables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 1) {

        std::cout << colorTextByTerritory(i,j,gameVariables) << "✪";

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
    std::cout << "\n";

}

void textRenderer::render (int currentCivilizationIndex, int turnNumber, GameVariables &gameVariables) {

    for (int i = 0; i < worldMap.worldSize; i++){

        for (int j = 0; j < worldMap.worldSize*4; j++){

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

            for (int i = 0; i < worldMap.worldSize; i++){
            for (int j = 0; j < worldMap.worldSize*4; j++){
            if (isUnitAtPosition(i,j, gameVariables) == false && isCityAtPosition(i, j, gameVariables) == false) {

                std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j,gameVariables) << renderTextBasedOnWorldFeature(i,j); /*flag (r:should it just cout in the function?*/

            } else if (isUnitAtPosition(i,j,gameVariables)){

                std::cout << colorTextByTerritory(i,j,gameVariables) << colorTextByCity(i,j,gameVariables);
                DisplayUnit (i,j);

            } else if (isCityAtPosition(i,j,gameVariables) == true) {

                std::cout << colorTextByTerritory(i,j,gameVariables) << "✪";

            }
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

