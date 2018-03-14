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

        void think (int g, GameVariables &game_variables, std::vector<Trade> &trades);
        void moveAllUnitsBelongingToCiv (int g, GameVariables &game_variables);

        int returnTradeValue (Trade trade, GameVariables &gameVariables);

        WorldMap worldMap;

    protected:
    private:

        void mapUnitPathToRuin (int civilizationIndex, GameVariables &game_variables, int unitIndex);

        int returnUnexploredTiles (int x, int y, int g, int range, GameVariables &game_variables);

        int checkForUnexploredTerritory (int unitIndex, int g, GameVariables &game_variables);

        void moveUnit (int unitIndex, int g, GameVariables &game_variables);

        int calculateResearchPriority (Research tech, int g, GameVariables &game_variables);

        void setTechToResearch (int g, int techIndex, GameVariables &game_variables);

        void setResearchPriority (int g, GameVariables &game_variables);

        int returnUnitPotential (int g, Unit unit, GameVariables &game_variables);

        void produceUnit (int g, int cityIndex, std::string unitName, GameVariables &game_variables);

        int returnBuildingPotential (int g, Building building, GameVariables &game_variables);

        void produceBuilding (int g, int cityIndex, std::string buildingName, GameVariables &game_variables);

        void produce (int g, int cityIndex, GameVariables &game_variables);

        bool hasLowHappiness (int g, GameVariables &game_variables);

        void tradingLogic (int g, GameVariables &game_variables, std::vector<Trade> &trades);

};

#endif // AI_H
