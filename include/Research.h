#ifndef RESEARCH_H
#define RESEARCH_H

#include <vector>
#include <string>

class Research
{
    public:
        Research() {}
        virtual ~Research() {}

        std::string researchName;

        int scienceCostToLearnResearch;

        std::vector<std::string> prerequisiteTechnologiesRequired;

        /// AI Focuses tell the AI what the technology will help with, and the value ranges from 0 (unimportant) to 10 (high priority)
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
        /// This focus tells the AI how important the technology is no matter what type of AI it is.
        /// For example, Embarking allows units to cross coasts and oceans, a major step up important to all AI.

        std::vector<std::string> unlockableUnits;

        void resetResearchfocus () {
 /*flag (R: unnecessary after tests*/
            aiFocus_offense = 0;
            aiFocus_defense = 0;
            aiFocus_economic = 0;
            aiFocus_population = 0;
            aiFocus_production = 0;
            aiFocus_diplomatic = 0;
            aiFocus_scientific = 0;
            aiFocus_exploration = 0;
            aiFocus_religion = 0;
            aiFocus_overall_importance = 1;

        }

    protected:
    private:
};

#endif // RESEARCH_H
