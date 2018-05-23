#include "Event.h"
#include "sharedMethods.h"
#include "Alliance.h"
#include <iostream>

Event::Event()
{
    //ctor
}

Event::~Event()
{
    //dtor
}

enum choiceResponseCodes : int {

    YES = 1,
    NO = 2,

};

void Event::listen (int civilizationIndex, std::vector<Civilization> &Civilizations) {

    if (test_canBeTriggered == true && targetCivilizationIndex == civilizationIndex) {

        if (Civilizations[civilizationIndex].playedByHumans == true) {

            trigger (Civilizations);

        } else {

            trigger_ai (Civilizations);

        }

        hasBeenTriggered = true;

    }

}

void Event::responseLogic (int Choice, std::vector<Civilization> &Civilizations) {

}

void Event::trigger (std::vector<Civilization> &Civilizations) {

    std::cout << "\n\n[====== " << EventName << " ======]" << std::endl;
    std::cout << "\n" << EventMessage << std::endl;

    for (unsigned int i = 1; i <= ResponseChoices.size(); i++) {

        std::cout << "[" << i << "] " << ResponseChoices[i-1] << std::endl;

    }

    if (ResponseChoices.size() >= 1) {

        int Choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

        responseLogic(Choice, Civilizations);

    }
}

void Event::trigger_ai (std::vector<Civilization> &Civilizations) {

    int Choice = 0;
    if (eventType == "alliance") {

        if (Civilizations[targetCivilizationIndex].relationsWithOtherCivilizations[initializerCivilization] >= -10) {

            Choice = choiceResponseCodes::YES;

        } else {

            Choice = choiceResponseCodes::NO;

        }

    }

    responseLogic(Choice, Civilizations);

}
