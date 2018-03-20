#ifndef GAMEUPDATER_H
#define GAMEUPDATER_H
#include "GameVariables.h"

class GameUpdater
{
    public:
        GameUpdater();
        virtual ~GameUpdater();

        void updateResources (int civilizationIndex, GameVariables &gameVariables);

        void updateCities (GameVariables &gameVariables);

        bool canCityExpand (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void expandCityTerritory (int cityIndex, GameVariables &gameVariables);

        void updateCityBuildingProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void updateCityUnitProduction (int cityIndex, int civilizationIndex, GameVariables &gameVariables);

        void updateCityProductionModifier (int cityIndex, int civilizationIndex, GameVariables &gameVariables);
    protected:
    private:
};

#endif // GAMEUPDATER_H
