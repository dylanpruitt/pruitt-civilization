#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "WorldMap.h"
#include "GameVariables.h"

class textRenderer
{
    public:
        textRenderer();
        virtual ~textRenderer();


        WorldMap worldMap;

        void render(int currentCivilizationIndex, int turnNumber, GameVariables &game_variables);

        void spectate (int turnNumber, GameVariables &game_variables);


    protected:
    private:

        void DisplayUnit (int i, int j);

        bool isUnitAtPosition (int x, int y, GameVariables &game_variables);
        bool isCityAtPosition (int x, int y, GameVariables &game_variables);

        int returnCivMilitaryPower (int civ_index, GameVariables &game_variables);

        std::string colorTextByTerritory(int i, int j);
        std::string colorTextByCity (int i, int j, GameVariables &game_variables);

        std::string renderTextBasedOnWorldFeature (int i, int j);
        void DisplayTextBasedOnEntitiesAtPosition (int i, int j, int currentCivilizationIndex, GameVariables &game_variables);

        void renderStatusTextRightOfWorldMap (int i, int currentCivilizationIndex, int turnNumber, GameVariables &game_variables);


};

#endif // TEXTRENDERER_H
