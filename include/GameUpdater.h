#ifndef GAMEUPDATER_H
#define GAMEUPDATER_H
#include <vector>

#include "GameVariables.h"
#include "Event.h"
#include "Trade.h"
#include "AI.h"

class GameUpdater
{
    public:
        GameUpdater();
        virtual ~GameUpdater();

        void updateResources (int civilizationIndex, GameVariables &gameVariables);

        void updateCities (GameVariables &gameVariables);

        void updateForCivilization (int civilizationIndex, GameVariables &gameVariables, AI ai);

        void UpdateCivilizationExploredTerritory (int civilizationIndex, GameVariables &gameVariables);

        void UpdateAllUnitsMovement (GameVariables &gameVariables);

        void updateResearch (int civilizationIndex, GameVariables &gameVariables);

    protected:
    private:

        bool canCityExpand (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void expandCityTerritory (int cityIndex, GameVariables &gameVariables);

        void updateCityBuildingProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void updateCityUnitProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void updateCityProductionModifier (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void updateGoldPerTurn (int civilizationIndex, GameVariables &gameVariables);

        void updateTrades (int civilizationIndex, GameVariables &gameVariables, AI ai);

        void updateCivilizationHappiness (int civilizationIndex, GameVariables &gameVariables);

        void updateEvents (int civilizationIndex, GameVariables &gameVariables);

        bool unitIsNotAlreadyUnlocked (int civilizationIndex, std::string unitName, GameVariables &gameVariables);

        void unlockUnitsFromResearchCompletion (Research research, int civilizationIndex, GameVariables &gameVariables);

        void promoteUnitsToAllowCoastalEmbarkment (int civilizationIndex, GameVariables &gameVariables);
};

#endif // GAMEUPDATER_H
