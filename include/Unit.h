#ifndef UNIT_H
#define UNIT_H

#include "Civilization.h"
#include "Domain.h"
#include "Position.h"
#include "TerrainModifier.h"
#include "WorldMap.h"

#include <vector>

class Unit
{
    public:
        Unit() {}
        virtual ~Unit() {}

        std::string name;

        Position position;

        int parentCivilizationIndex = 0, parentGroupingIndex = -1;

        int health = 10; int maxHealth = 10;

        int combatStrength = 5;

        int terrainMoveCost = 2;

        TerrainModifier grasslandModifier;
        TerrainModifier mountainModifier;
        TerrainModifier forestModifier;
        TerrainModifier snowModifier;
        TerrainModifier desertModifier;

        int movementPoints = 2;

        int maxMovePoints = 2;

        int productionCost = 30;

        int goldCost = 80;

        bool isRanged = false;

        int rangedCombat = 0;

        int range = 2;

        Domain domain;

        int aiFocus_offense = 0;
        int aiFocus_defense = 0;
        int aiFocus_economic = 0;
        int aiFocus_exploration = 0;

        int aiFocus_overall_importance = 1;

        std::vector<Position> moveQueue;

        std::vector<std::string> accolades;

        bool destinationHasBeenAssigned = false;

        bool isTraining = false;

        void seizeTerritory (WorldMap &worldMap) {

            worldMap.WorldTerritoryMap[position.x][position.y] = parentCivilizationIndex+1;

        }

    protected:
    private:

};

#endif // UNIT_H
