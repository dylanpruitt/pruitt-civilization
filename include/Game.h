#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <random>
#include <math.h>
#include <time.h>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>

#include "Unit.h"
#include "GameVariables.h"
#include "Trade.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        void generateCiv(int Civ);

        void setupWorld();

        void loop();

        GameVariables gameVariables;

    protected:
    private:

        void loadTechnologiesFromFile (std::string filename, int civilizationIndex);

        void loadUnitsFromFile (std::string filename);

        void loadBuildingsFromFile (std::string filename);

        void loadCivilizationsFromFile (std::string filename, std::vector<Civilization> &civs);

        /**
        *
        * MAP FUNCTIONS
        *
        **/
        bool isLandTile(int x, int y);

        int returnLandTiles(int x, int y);

        int getCivilizationPopulation (int civilizationIndex);

        int getTileFoodYield (int i, int j);

        int getTileProductionYield (int i, int j);

        int returnTerritoryTiles(int x, int y, int Civ_index);

        int returnUnexploredTiles (int x, int y, int civilizationIndex, int range);

        int getTerritory(int Civ_index);

        void foundCity (int x, int y, int CivilizationIndex);

        void promoteUnitsToAllowCoastalEmbarkment (int civilizationIndex);

        void giveCivilizationAncientRuinYield (int civilizationIndex);
        /**
        *
        * RESEARCH FUNCTIONS
        *
        **/

        bool isResearchComplete (int civilizationIndex);

        bool unitIsNotAlreadyUnlocked (int civilizationIndex, std::string unitName);

        void unlockUnitsFromResearchCompletion (Research research, int civilizationIndex);

        void updateResearch (int civilizationIndex);

        void pickResearchForHumanPlayer (int civilizationIndex);

        bool civilizationHasTechnology (int civilizationIndex, std::string techName);

        /**
        *
        * CORE GAME FUNCTIONS
        *
        **/

        bool isTileBorderingCivilizationTerritory (int x, int y, int civilizationIndex);
        bool canCityExpand (int cityIndex, int civilizationIndex);
        void expandCityTerritory (int cityIndex);
        void UpdateCivilizationExploredTerritory (int civilizationIndex);
        void DisplayCivilizationExplorationProgress (int civilizationIndex);
        void DisplayCitiesStatusesOwnedByCivilization (int civilizationIndex);
        void getPlayerChoiceAndReact (int civilizationIndex);

        void showAvailableUnits (int civilizationIndex, bool forProduction, bool forBuying);

        void showAvailableBuildings (int cityIndex);

        void buyUnits (int civilizationIndex);

        void produceUnits (int cityIndex, int civilizationIndex);

        void produceBuilding (int cityIndex, int civilizationIndex);

        void setupDiplomacy ();

        void checkTileForResource (int civilizationIndex, int x, int y);

        void setCityProduction (int cityIndex);

        void assignWorkByPopulation (int cityIndex, bool stopAfterNeededAmountIsCollected);

        void assignPopulationToTiles (int cityIndex, int focusCode);

        void updateCityProductionModifier (int cityIndex, int civilizationIndex);

        void updateCityUnitProduction (int cityIndex, int civilizationIndex);

        void updateCityBuildingProduction (int cityIndex, int civilizationIndex);

        void requestAlliance (int civilizationIndex, int targetCivilizationIndex);

        void playerRequestAlliance (int civilizationIndex);

        void updateEvents (int civilizationIndex);

        void updateCivilizationHappiness (int civilizationIndex);

        void updateResources (int civilizationIndex);

        void updateCities ();

        void displayCivilizationResources (int civilizationIndex);

        int inputResourcesToTrade (int civilizationIndex, Trade &trade);

        bool resourceIsNotAlreadyBeingTraded (int civilizationIndex, int resourceIndex, std::vector<Resource> resources);

        void editTrade (int civilizationIndex, Trade &trade, int Choice);

        void trade (int civilizationIndex);

        void updateTrades (int civilizationIndex);

        void updateGoldPerTurn (int civilizationIndex);

        void displayUnitDetails (Unit unit);

};

#endif // GAME_H
