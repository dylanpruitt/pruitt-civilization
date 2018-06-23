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

        std::string focus = "none";

        std::string CivName;
        std::string CivilizationAdjective;

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

        int rgbValues[3];

        std::vector<std::string> learnedTechnologies;

        std::vector<std::string> AvailableUnitsToCreate;

        std::vector<int> relationsWithOtherCivilizations;

        std::vector<bool> hasMetCivilizations;

        std::vector<Resource> resources;

        Research technologyBeingResearched;

        int researchPoints = 0;

        int aiFocus_offense = 5;
        int aiFocus_defense = 5;
        int aiFocus_economic = 5;
        int aiFocus_population = 5;
        int aiFocus_production = 5;
        int aiFocus_diplomatic = 5;
        int aiFocus_scientific = 5;
        int aiFocus_exploration = 5;
        int aiFocus_religion = 5;

        int ai_fairness = 5;

        int GoldPerTurn = 5;

        std::vector<Research> technologiesToResearch;

        std::vector<std::string> cityNames;

        std::vector<UnitGrouping> unitGroups;

        bool isAtWar = false;

        void addNewGrouping (std::string name, int rgb[3]) {

            UnitGrouping newGroup;

            newGroup.name = name;

            for (int i = 0; i < 3; i++) {

                newGroup.rgbValue[i] = rgb[i];

            }

            unitGroups.push_back (newGroup);

        }

        bool hasMetCivilization (int civilizationIndex) {

            if (civilizationIndex >= 0) {

                if (hasMetCivilizations[civilizationIndex] == true) {

                    return true;

                } else {

                    return false;

                }

            }

            return false;
        }

};

#endif // CIVILIZATION_H
