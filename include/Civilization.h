#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <vector>

#include "Research.h"
#include "Resource.h"
#include "UnitGrouping.h"

struct Civilization
{
    public:

        enum eras : int {

            ANCIENT = 1,
            CLASSICAL = 2,
            MEDIEVAL = 3,
            RENAISSANCE = 4,
            COLONIAL = 5,
            INDUSTRIAL = 6,
            MODERN = 7,
            FUTURE = 8,

        };

        int era = eras::ANCIENT;

        std::string CivName;

        int startingX; int startingY; int Gold = 125;

        int Happiness = 75;

        double ExpansionRate = 1.00;

        double ScienceRate = 1.00;

        int WorldExplorationMap[50][200];

        int upkeepCostPerUnit = 1;

        int baseWarSupportPercentage = 75;

        int warSupportPercentage = 75;

        double unitAttackModifier = 1.0;
        double unitDefenseModifier = 1.0;

        bool playedByHumans = false;

        int rgbValues[3] = {0, 0, 0};

        std::vector<std::string> learnedTechnologies;

        std::vector<std::string> AvailableUnitsToCreate;

        std::vector<int> relationsWithOtherCivilizations;

        std::vector<Resource> resources;

        Research technologyBeingResearched;

        int researchPoints = 0;

        int aiFocus_offense = 0;
        int aiFocus_defense = 0;
        int aiFocus_economic = 0;
        int aiFocus_population = 0;
        int aiFocus_production = 0;
        int aiFocus_diplomatic = 0;
        int aiFocus_scientific = 0;
        int aiFocus_exploration = 0;
        int aiFocus_religion = 0;

        int ai_fairness = 0;

        int GoldPerTurn = 5;

        std::vector<Research> technologiesToResearch;

        std::vector<std::string> cityNames;

        std::vector<UnitGrouping> unitGroups;

        void addNewGrouping (std::string name, int rgb[3]) {

            UnitGrouping newGroup;

            newGroup.name = name;

            for (int i = 0; i < 3; i++) {

                newGroup.rgbValue[i] = rgb[i];

            }

            unitGroups.push_back (newGroup);

        }

};

#endif // CIVILIZATION_H
