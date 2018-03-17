#ifndef SHAREDMETHODS_H
#define SHAREDMETHODS_H
#include <string>
#include "Civilization.h"
#include "GameVariables.h"
#include "WorldMap.h"
#include "Trade.h"

namespace sharedMethods
{

        extern int returnAcceptableIntegerInput ();

        extern int bindIntegerInputToRange (int minimumAcceptableValue, int maximumAcceptableValue, int defaultValue);

        extern int getDistance(int x, int y, int x2, int y2);

        extern int getUnitIndexByName (std::string name, GameVariables &gameVariables);

        extern bool unitIsNotTrespassing (int civilizationIndex, int xPositionToMoveTo, int yPositionToMoveTo, WorldMap worldMap);

        extern void moveUnit (Unit &unit, int xPositionToMoveTo, int yPositionToMoveTo, Civilization &civ, WorldMap worldMap);

        bool UnitisOnAnAncientRuin (Unit &unit, WorldMap worldMap);

        void getAncientRuinBenefits (Unit &unit, Civilization &civ, WorldMap worldMap);

        extern int getBuildingIndexByName (std::string name, GameVariables &gameVariables);

        extern int getResearchIndexByName (int civilizationIndex, std::string techName, GameVariables &gameVariables);

        extern bool CivilizationHasPrerequisiteTechs (int civilizationIndex, std::string techName, GameVariables &gameVariables);

};

#endif // SHAREDMETHODS_H
