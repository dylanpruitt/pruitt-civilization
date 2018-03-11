#ifndef UNIT_H
#define UNIT_H

#include "Civilization.h"
#include "Position.h"
#include "TerrainModifier.h"

<<<<<<< HEAD
#include <vector>

=======
>>>>>>> e640d05feb606a4369aee60633a48af9bc130064
class Unit
{
    public:
        Unit() {}
        virtual ~Unit() {}

        std::string name;

        Position position;

        int parentCivilizationIndex = 0;

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

        int aiFocus_offense = 0;
        int aiFocus_defense = 0;
        int aiFocus_economic = 0;
        int aiFocus_exploration = 0;

        int aiFocus_overall_importance = 1;

<<<<<<< HEAD
        std::vector<int> moveDirectionQueue;

=======
>>>>>>> e640d05feb606a4369aee60633a48af9bc130064
    protected:
    private:
};

#endif // UNIT_H
