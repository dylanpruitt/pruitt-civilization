#include "Event.h"
#include "sharedMethods.h"
#include <iostream>

Event::Event()
{
    //ctor
}

Event::~Event()
{
    //dtor
}

enum choiceResponseCodes {

    YES = 1,
    NO = 2,

};

void Event::listen (int civilizationIndex, GameVariables &gameVariables) {

    if (test_canBeTriggered == true && targetCivilizationIndex == civilizationIndex) {

        if (gameVariables.Civilizations[civilizationIndex].playedByHumans == true) {

            trigger (gameVariables);

        } else {

            trigger_ai (gameVariables);

        }

        hasBeenTriggered = true;

    }

}

void Event::responseLogic (int Choice, GameVariables &gameVariables) {

    if (eventType == "alliance") {

        if (Choice == choiceResponseCodes::YES) {

            gameVariables.Civilizations[targetCivilizationIndex].relationsWithOtherCivilizations[initializerCivilization] += 30;
            gameVariables.Civilizations[initializerCivilization].relationsWithOtherCivilizations[targetCivilizationIndex] += 30;

        }

    }

}

void Event::trigger (GameVariables &gameVariables) {

    std::cout << "\n\n[====== " << EventName << " ======]" << std::endl;
    std::cout << "\n" << EventMessage << std::endl;

    for (unsigned int i = 1; i <= ResponseChoices.size(); i++) {

        std::cout << "[" << i << "] " << ResponseChoices[i-1] << std::endl;

    }

    if (ResponseChoices.size() >= 1) {

        int Choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

        responseLogic(Choice, gameVariables);

    }
}

void Event::trigger_ai (GameVariables &gameVariables) {

    int Choice = 0;
    if (eventType == "alliance") {

        if (gameVariables.Civilizations[targetCivilizationIndex].relationsWithOtherCivilizations[initializerCivilization] >= -10) {

            Choice = choiceResponseCodes::YES;

        } else {

            Choice = choiceResponseCodes::NO;

        }

    }

    responseLogic(Choice, gameVariables);

}
