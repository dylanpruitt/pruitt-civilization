#include "Game.h"
#include <string>
#include <iostream>

void showTitle () {

std::cout << "Pruitt's Linux Exploration Based Simulation (PLEBS)\n_        _\n\\\\      //\n \\\\    //__  _   _     __\n  \\\\  //|   | | |   | |  | |\\  |\n   \\\\// |-- |-  |_  | |  | | \\ |\n    \\/  |__ | \\ __| | |__| |  \\|  o.o.6\nCradle of Civilizations\n\n";

}

int main()
{
    srand(time(NULL));

    Game game;

    std::cout << "\033]0;PLEBS Version 0.0.6 - Cradle of Civilizations\007" << std::endl; /*flag (r:windows port will need SetConsoleTitle()*/

    showTitle();

    game.setupWorld();

    game.loop();
    return 0;
}
