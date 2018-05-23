#ifndef WAR_H
#define WAR_H

#include <vector>

#include "Civilization.h"

struct War
{
    public:

        std::vector<Civilization> offenderCivilizations;

        std::vector<int> offenderCivilizationWarScores;



        std::vector<Civilization> defenderCivilizations;

        std::vector<int> defenderCivilizationWarScores;

        std::string name;

    protected:
    private:
};

#endif // WAR_H
