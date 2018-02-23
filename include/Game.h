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

        void loadTechnologiesFromFile (std::string filename, int g);

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

        int getCivilizationPopulation (int g);

        int getTileFoodYield (int i, int j);

        int getTileProductionYield (int i, int j);

        int returnTerritoryTiles(int x, int y, int Civ_index);

        int returnUnexploredTiles (int x, int y, int g, int range);

        int getTerritory(int Civ_index);

        void foundCity (int x, int y, int CivilizationIndex);

        void promoteUnitsToAllowCoastalEmbarkment (int g);

        bool IsUnitIsOnAncientRuin (int unitIndex);

        void giveCivilizationAncientRuinYield (int g);
        /**
        *
        * RESEARCH FUNCTIONS
        *
        **/

        bool isResearchComplete (int g);

        void updateResearch (int g);

        void pickResearchForHumanPlayer (int g);

        bool civilizationHasTechnology (int g, std::string techName);

        /**
        *
        * CORE GAME FUNCTIONS
        *
        **/

        void CivilizationCheckAndExpandTerritory ();
        void UpdateCivilizationExploredTerritory (int g);
        void DisplayCivilizationExplorationProgress (int g);
        void DisplayCitiesStatusesOwnedByCivilization (int g);
        void getPlayerChoiceAndReact (int g);

        void showAvailableUnits (int g, bool forProduction, bool forBuying);

        void showAvailableBuildings (int cityIndex);

        void buyUnits (int g);

        void produceUnits (int cityIndex, int g);

        void produceBuilding (int cityIndex, int g);

        void setupDiplomacy ();

        void checkTileForResource (int g, int x, int y);

        void setCityProduction (int cityIndex);

        void assignWorkByPopulation (int cityIndex, bool stopAfterNeededAmountIsCollected);

        void assignPopulationToTiles (int cityIndex, int focusCode);

        void updateCityProductionModifier (int cityIndex, int g);

        void updateCityUnitProduction (int cityIndex, int g);

        void updateCityBuildingProduction (int cityIndex, int g);

        /**
        *
        * MISC GAME VARIABLES
        *
        **/

        void requestAlliance (int g, int targetCivilizationIndex);

        void playerRequestAlliance (int g);

        void updateEvents (int g);

        void updateCivilizationHappiness (int g);

        void updateResources (int g);

        void updateCities ();

        void displayCivilizationResources (int g);

        int inputResourcesToTrade (int g, Trade &trade);

        bool resourceIsNotAlreadyBeingTraded (int g, int resourceIndex, std::vector<Resource> resources);

        void editTrade (int g, Trade &trade, int Choice);

        void trade (int g);

        void updateTrades (int g);

};

#endif // GAME_H
