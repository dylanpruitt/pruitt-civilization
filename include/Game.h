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

        void generateMinorCiv (int Civ);

        void setupWorld();

        void loop();

        GameVariables gameVariables;

    protected:
    private:

        bool startLocationIsValid (int x, int y);

        void setNewCivilizationExploration ();

        void spawnInNewCivilization (std::vector<Civilization> civs);

        void loadTechnologiesFromFile (std::string filename, int civilizationIndex);

        void loadUnitsFromFile (std::string filename);

        void loadBuildingsFromFile (std::string filename);

        void loadCivilizationsFromFile (std::string filename, std::vector<Civilization> &civs);

        void loadMinorCivilizationsFromFile (std::string filename, std::vector<Civilization> &civs);

        void saveGame (std::string filename);

        bool isLandTile(int x, int y);

        int returnLandTiles(int x, int y);

        int getCivilizationPopulation (int civilizationIndex);

        int getTileFoodYield (int i, int j);

        int getTileProductionYield (int i, int j);

        int returnTerritoryTiles(int x, int y, int Civ_index);

        int returnUnexploredTiles (int x, int y, int civilizationIndex, int range);

        void giveCivilizationAncientRuinYield (int civilizationIndex);

        void pickResearchForHumanPlayer (int civilizationIndex);

        int returnNumberOfTilesExploredByCivilization (int civilizationIndex);

        bool isTileBorderingCivilizationTerritory (int x, int y, int civilizationIndex);
        bool canCityExpand (int cityIndex, int civilizationIndex);
        void expandCityTerritory (int cityIndex);
        void DisplayCivilizationExplorationProgress (int civilizationIndex);
        void DisplayCitiesStatusesOwnedByCivilization (int civilizationIndex);
        void getPlayerChoiceAndReact (int civilizationIndex);

        void showAvailableUnits (int civilizationIndex, int cityIndex, bool forProduction, bool forBuying);

        void showAvailableBuildings (int cityIndex);

        void buyUnits (int civilizationIndex, int cityIndex);

        void produceUnits (int cityIndex, int civilizationIndex);

        void produceBuilding (int cityIndex, int civilizationIndex);

        void setupDiplomacy ();

        void checkTileForResource (int civilizationIndex, int x, int y);

        void setCityProduction (int cityIndex);

        void requestAlliance (int civilizationIndex, int targetCivilizationIndex);

        void playerRequestAlliance (int civilizationIndex);

        void displayCivilizationResources (int civilizationIndex);

        int inputResourcesToTrade (int civilizationIndex, Trade &trade);

        bool resourceIsNotAlreadyBeingTraded (int civilizationIndex, int resourceIndex, std::vector<Resource> resources);

        void editTrade (int civilizationIndex, Trade &trade, int Choice);

        void trade (int civilizationIndex);

        void displayUnitDetails (Unit unit);

        int getTerrainCodeUnitIsOn (Unit &unit);

        void updateUnitTraining ();

        void setUnitUpkeep (int civilizationIndex);

        double calculateUnitAttackingModifier (Unit &attacker, Unit &defender);

        double calculateUnitDefendingModifier (Unit &defender);

        int returnUnitIndexFromPosition (int civilizationIndex, int x, int y);

        void combat (Unit &attacker, Unit &defender);

        void openTradingMenu (int civilizationIndex);

        void offerLoan (int civilizationIndex);

        void editLoan (int civilizationIndex, Loan &loan);

};

#endif // GAME_H
