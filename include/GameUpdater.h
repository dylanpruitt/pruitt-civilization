#ifndef GAMEUPDATER_H
#define GAMEUPDATER_H
#include <vector>

#include "GameVariables.h"
#include "Event.h"
#include "Trade.h"
#include "AI.h"

namespace GameUpdater
{

        extern void updateResources (int civilizationIndex, GameVariables &gameVariables);

        extern void updateCities (GameVariables &gameVariables);

        extern void updateForCivilization (int civilizationIndex, GameVariables &gameVariables, AI ai);

        extern void UpdateCivilizationExploredTerritory (int civilizationIndex, GameVariables &gameVariables);

        extern void UpdateAllUnitsMovement (GameVariables &gameVariables);

        extern void updateResearch (int civilizationIndex, GameVariables &gameVariables);

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

        void promoteUnitsFromResearchCompletion (Research research, int civilizationIndex, GameVariables &gameVariables);

        void updateEffectsFromUnitUpkeep (int civilizationIndex, GameVariables &gameVariables);
};

#endif // GAMEUPDATER_H
