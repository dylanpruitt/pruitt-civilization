#ifndef WAR_H
#define WAR_H

#include <vector>

#include "Civilization.h"

struct War
{
    public:

        std::vector<int> offenderCivilizationIndices;

        std::vector<int> defenderCivilizationIndices;

        std::string name;

    protected:
    private:
};

#endif // WAR_H
