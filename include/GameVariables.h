#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H
#include <vector>
#include "Civilization.h"
#include "City.h"
#include "Unit.h"

class GameVariables
{
    public:
        GameVariables();
        virtual ~GameVariables();

        std::vector<Civilization> Civilizations;

        std::vector<Unit> UnitsInGame;

        std::vector<City> Cities;

        std::vector<Unit> Units;

        std::vector<Building> Buildings;

    protected:
    private:
};

#endif // GAMEVARIABLES_H
