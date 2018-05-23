#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H
#include <vector>
#include "City.h"
#include "Unit.h"
#include "WorldMap.h"
#include "LoanEvent.h"
#include "Trade.h"
#include "Loan.h"
#include "Alliance.h"

struct GameVariables
{

        std::vector<Civilization> Civilizations;

        std::vector<Unit> UnitsInGame;

        std::vector<City> Cities;

        std::vector<Unit> Units;

        std::vector<Building> Buildings;

        std::vector<Event> gameEvents;

        std::vector<Trade> trades;

        std::vector<Loan> activeLoans;

        std::vector<Alliance> alliances;

        WorldMap worldMap;

};

#endif // GAMEVARIABLES_H
