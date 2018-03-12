#ifndef BUILDING_H
#define BUILDING_H
#include <string>

struct Building
{

    std::string name;

    std::string description;

    int FoodYield;
    int ProductionYield;
    int GoldYield;
    int ScienceYield;
    int FaithYield;

    int ProductionCost = 40;
    int GoldCost = 150;

    int aiFocus_offense = 0;
    int aiFocus_defense = 0;
    int aiFocus_economic = 0;
    int aiFocus_population = 0;
    int aiFocus_production = 0;
    int aiFocus_diplomatic = 0;
    int aiFocus_scientific = 0;
    int aiFocus_exploration = 0;
    int aiFocus_religion = 0;
    int aiFocus_overall_importance = 1;
};

#endif // BUILDING_H
