#ifndef AI_H
#define AI_H
#include "Civilization.h"
#include "GameVariables.h"
#include "WorldMap.h"
#include "Trade.h"

class AI
{
    public:
        AI();
        virtual ~AI();

        void think (int civilizationIndex, GameVariables &gameVariables, std::vector<Trade> &trades);
        void moveAllUnitsBelongingToCiv (int civilizationIndex, GameVariables &gameVariables);

        int returnTradeValue (Trade trade, GameVariables &gameVariables);

    protected:
    private:

        void mapUnitPathToRuin (int civilizationIndex, GameVariables &gameVariables, int unitIndex);

        int returnUnexploredTiles (int x, int y, int civilizationIndex, int range, GameVariables &gameVariables);

        int checkForUnexploredTerritory (int unitIndex, int civilizationIndex, GameVariables &gameVariables);

        void moveUnit (int unitIndex, int civilizationIndex, GameVariables &gameVariables);

        int calculateResearchPriority (Research tech, int civilizationIndex, GameVariables &gameVariables);

        void setTechToResearch (int civilizationIndex, int techIndex, GameVariables &gameVariables);

        void setResearchPriority (int civilizationIndex, GameVariables &gameVariables);

        int returnUnitPotential (int civilizationIndex, Unit unit, GameVariables &gameVariables);

        void produceUnit (int civilizationIndex, int cityIndex, std::string unitName, GameVariables &gameVariables);

        int returnBuildingPotential (int civilizationIndex, Building building, GameVariables &gameVariables);

        void produceBuilding (int civilizationIndex, int cityIndex, std::string buildingName, GameVariables &gameVariables);

        void produce (int civilizationIndex, int cityIndex, GameVariables &gameVariables);

        bool hasLowHappiness (int civilizationIndex, GameVariables &gameVariables);

        void tradingLogic (int civilizationIndex, GameVariables &gameVariables, std::vector<Trade> &trades);

};

#endif // AI_H
