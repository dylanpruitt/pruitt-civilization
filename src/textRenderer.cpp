#include "textRenderer.h"
#include <iostream>

textRenderer::textRenderer()
{
    //ctor
}

textRenderer::~textRenderer()
{
    //dtor
}

bool textRenderer::isUnitAtPosition (int x, int y, GameVariables &game_variables) {

    for (unsigned int i = 0; i < game_variables.UnitsInGame.size(); i++) {

        if (game_variables.UnitsInGame[i].position.x == x && game_variables.UnitsInGame[i].position.y == y) {

            return true;

        }

    }

}

bool textRenderer::isCityAtPosition (int x, int y, GameVariables &game_variables) {

    for (unsigned int i = 0; i < game_variables.Cities.size(); i++) {

        if (game_variables.Cities[i].position.x == x && game_variables.Cities[i].position.y == y) {

            return true;

        }

    }

    return false;

}

int textRenderer::returnCivMilitaryPower (int civ_index, GameVariables &game_variables) {

    int militarypower = 0;

    for (unsigned int i = 0; i < game_variables.UnitsInGame.size(); i++) {

        if (game_variables.UnitsInGame[i].parentCivilizationIndex == civ_index) {

            if (game_variables.Civilizations[civ_index].LeaderTrait != "Brave") {
                militarypower += (game_variables.UnitsInGame[i].combat * game_variables.UnitsInGame[i].combat);
            } else {
                militarypower += ((game_variables.UnitsInGame[i].combat*1.15) * (game_variables.UnitsInGame[i].combat*1.15));
            }

        }

    }

    return militarypower;

}


std::string textRenderer::colorTextByTerritory (int i, int j) {

    if (worldMap.WorldTerritoryMap[i][j] == 1) {
        return "\033[0;45m";
    } else if (worldMap.WorldTerritoryMap[i][j] == 2) {
        return "\033[0;104m";
    } else if (worldMap.WorldTerritoryMap[i][j] == 3) {
        return "\033[0;42m";
    } else if (worldMap.WorldTerritoryMap[i][j] == 4) {
        return "\033[0;100m";
    } else if (worldMap.WorldTerritoryMap[i][j] == 5) {
        return "\033[0;41m";
    } else {
        return "";
    }
}

std::string textRenderer::colorTextByCity (int i, int j, GameVariables &game_variables) {
    if (isCityAtPosition(i,j, game_variables)) {
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

void textRenderer::DisplayTextBasedOnEntitiesAtPosition (int i, int j, int currentCivilizationIndex, GameVariables &game_variables) {

    if (game_variables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 1 && isUnitAtPosition(i, j, game_variables) == false && isCityAtPosition(i, j,game_variables) == false) {

        std::cout << colorTextByTerritory(i,j) << colorTextByCity(i,j, game_variables) << renderTextBasedOnWorldFeature(i,j); /*flag (r:should it just cout in the function?*/


    } else if (game_variables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 0) {

        if(i % 2 == 0 && j % 2 == 0) { std::cout << "\e[47;30m?\e[0m"; } else { std::cout << "\e[47m \e[0m"; }

    } else if (isUnitAtPosition(i,j, game_variables)){

        std::cout << colorTextByTerritory(i,j) << colorTextByCity(i,j, game_variables);
        DisplayUnit (i,j);

    } else if (isCityAtPosition(i,j, game_variables) == true && game_variables.Civilizations[currentCivilizationIndex].WorldExplorationMap[i][j] == 1) {

        std::cout << colorTextByTerritory(i,j) << "✪";

    }
}

void textRenderer::renderStatusTextRightOfWorldMap (int i, int currentCivilizationIndex, int turnNumber, GameVariables &game_variables) {

    std::cout << " |!|";

    if (i == 0) { std::cout << " - CIVILIZATIONS - "; }

    for (unsigned int w = 0; w < game_variables.Civilizations.size(); w++) {

        if (i == w+1 && i == currentCivilizationIndex+1) {
            std::cout << " > YOU < ";
        } else if (i == w+1) {
            std::cout << " " << game_variables.Civilizations[w].CivName << " - [" << game_variables.Civilizations[currentCivilizationIndex].relationsWithOtherCivilizations[w] << " | " << game_variables.Civilizations[w].relationsWithOtherCivilizations[currentCivilizationIndex] << "]";
        }

    }

    if (i == (game_variables.Civilizations.size() + 2)) { std::cout << " - TURN " << turnNumber << " - "; }

    if (i == (game_variables.Civilizations.size() + 5)) { std::cout << " - MILITARY POWER - "; }

    if (i == (game_variables.Civilizations.size() + 6)) { std::cout << " " << returnCivMilitaryPower(currentCivilizationIndex, game_variables);}

    if (i == (game_variables.Civilizations.size() + 7)) { std::cout << " - ECONOMICS -"; }

    if (i == (game_variables.Civilizations.size() + 8)) { std::cout << " GOLD : " << game_variables.Civilizations[currentCivilizationIndex].Gold; }

    if (i == (game_variables.Civilizations.size() + 9)) { std::cout << " - RESEARCH -"; }

    if (i == (game_variables.Civilizations.size() + 10)) { std::cout << " " << game_variables.Civilizations[currentCivilizationIndex].technologyBeingResearched.researchName; }

    if (i == (game_variables.Civilizations.size() + 11) && game_variables.Civilizations[currentCivilizationIndex].technologyBeingResearched.researchName != "") { std::cout << " " << game_variables.Civilizations[currentCivilizationIndex].researchPoints << " / " << game_variables.Civilizations[currentCivilizationIndex].technologyBeingResearched.scienceCostToLearnResearch; }

    if (i == (game_variables.Civilizations.size() + 12)) { std::cout << " HAPPINESS : " << game_variables.Civilizations[currentCivilizationIndex].Happiness; }
    std::cout << "\n";

}

void textRenderer::render (int currentCivilizationIndex, int turnNumber, GameVariables &game_variables) {

    for (int i = 0; i < worldMap.worldSize; i++){

        for (int j = 0; j < worldMap.worldSize*4; j++){

            DisplayTextBasedOnEntitiesAtPosition (i, j, currentCivilizationIndex, game_variables);

        }

        renderStatusTextRightOfWorldMap (i, currentCivilizationIndex, turnNumber, game_variables);

    }

}

void textRenderer::spectate (int turnNumber, GameVariables &game_variables) {

    bool stillSpectating = true;

    int currentCivIndex = 0;

    while (stillSpectating) {

        if (currentCivIndex > -1) {

            render (currentCivIndex, turnNumber, game_variables);

        } else {

            for (int i = 0; i < worldMap.worldSize; i++){
            for (int j = 0; j < worldMap.worldSize*4; j++){
            if (isUnitAtPosition(i,j, game_variables) == false && isCityAtPosition(i, j, game_variables) == false) {

                std::cout << colorTextByTerritory(i,j) << colorTextByCity(i,j,game_variables) << renderTextBasedOnWorldFeature(i,j); /*flag (r:should it just cout in the function?*/

            } else if (isUnitAtPosition(i,j,game_variables)){

                std::cout << colorTextByTerritory(i,j) << colorTextByCity(i,j,game_variables);
                DisplayUnit (i,j);

            } else if (isCityAtPosition(i,j,game_variables) == true) {

                std::cout << colorTextByTerritory(i,j) << "✪";

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

            if (currentCivIndex < (game_variables.Civilizations.size()-1) || currentCivIndex == -1) {

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

                currentCivIndex = game_variables.Civilizations.size()-1;

            }

        }

    }

}

