#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <vector>

#include "Research.h"
#include "Resource.h"

class Civilization
{
    public:
        Civilization() {}
        virtual ~Civilization() {}

        std::string CivName; std::string LeaderTrait;

        int startingX; int startingY; int Gold = 125;

        int Happiness = 75;

        double ExpansionRate = 1.00;

        double ScienceRate = 1.00;

        int WorldExplorationMap[50][200];

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


    protected:
    private:
};

#endif // CIVILIZATION_H
