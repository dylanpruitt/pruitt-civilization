#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "GameVariables.h"

class textRenderer
{
    public:
        textRenderer();
        virtual ~textRenderer();

        void render(int currentCivilizationIndex, int turnNumber, GameVariables &gameVariables);

        void spectate (int turnNumber, GameVariables &gameVariables);

        void DisplayUnitGroupings (int civilizationIndex, GameVariables &gameVariables);

    protected:
    private:

        void DisplayUnit (int i, int j);

        bool isUnitAtPosition (int x, int y, GameVariables &gameVariables);
        bool isCityAtPosition (int x, int y, GameVariables &gameVariables);

        int returnCivMilitaryPower (int civ_index, GameVariables &gameVariables);

        std::string colorTextByTerritory(int i, int j, GameVariables &gameVariables);
        std::string colorTextByCity (int i, int j, GameVariables &gameVariables);

        std::string renderTextBasedOnWorldFeature (int i, int j, GameVariables &gameVariables);

        void DisplayTextBasedOnFeature (int i, int j, GameVariables &gameVariables);

        void DisplayTextBasedOnEntitiesAtPosition (int i, int j, int currentCivilizationIndex, GameVariables &gameVariables);

        void renderStatusTextRightOfWorldMap (int i, int currentCivilizationIndex, int turnNumber, GameVariables &gameVariables);


};

#endif // TEXTRENDERER_H
