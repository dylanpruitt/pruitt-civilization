#include "Event.h"

Event::Event()
{
    //ctor
}

Event::~Event()
{
    //dtor
}

void Event::listen (int civilizationIndex, GameVariables &gameVariables) {

    if (this->targetCivilizationIndex == civilizationIndex) {

        if (gameVariables.Civilizations[civilizationIndex].playedByHumans == true) {

            this->trigger (gameVariables);

        } else {

            this->trigger_ai (gameVariables);

        }

        hasBeenTriggered = true;

    }

}

void Event::responseLogic (int Choice, GameVariables &gameVariables) {

}

void Event::trigger (GameVariables &gameVariables) {

    std::cout << "\n\n[====== " << this->EventName << " ======]" << std::endl;
    std::cout << "\n" << this->EventMessage << std::endl;

    for (unsigned int i = 1; i <= this->ResponseChoices.size(); i++) {

        std::cout << "[" << i << "] " << this->ResponseChoices[i-1] << std::endl;

    }

    if (ResponseChoices.size() >= 1) {

        int Choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

        this->responseLogic(Choice, gameVariables);

    }
}

void Event::trigger_ai (GameVariables &gameVariables) {

}
