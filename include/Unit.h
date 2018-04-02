#ifndef UNIT_H
#define UNIT_H

#include "Civilization.h"
#include "Position.h"
#include "TerrainModifier.h"

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

        int combat = 5;

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

        bool canCoastalEmbark = false;

        bool canCrossOceans = false;

        int aiFocus_offense = 0;
        int aiFocus_defense = 0;
        int aiFocus_economic = 0;
        int aiFocus_exploration = 0;

        int aiFocus_overall_importance = 1;

        std::vector<int> moveDirectionQueue;

        bool destinationHasBeenAssigned = false;

        bool isTraining = false;

    protected:
    private:

};

#endif // UNIT_H
