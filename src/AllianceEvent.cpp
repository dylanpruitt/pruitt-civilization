#include "AllianceEvent.h"

AllianceEvent::AllianceEvent()
{
    //ctor
}

AllianceEvent::~AllianceEvent()
{
    //dtor
}

void AllianceEvent::trigger (GameVariables &gameVariables) {

    std::cout << "\n\n[====== " << EventName << " ======]" << std::endl;
    std::cout << "\n" << EventMessage << std::endl;

    for (unsigned int i = 1; i <= ResponseChoices.size(); i++) {

        std::cout << "[" << i << "] " << ResponseChoices[i-1] << std::endl;

    }

    if (ResponseChoices.size() >= 1) {

        int Choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

        this->responseLogic (Choice, gameVariables);

    }
}

void AllianceEvent::responseLogic (int Choice, GameVariables &gameVariables) {

    if (Choice == choiceResponseCodes::YES) {

        Alliance newAlliance;

        newAlliance.name = gameVariables.Civilizations [initializerCivilization].CivilizationAdjective + " - "
            + gameVariables.Civilizations [targetCivilizationIndex].CivilizationAdjective + " Alliance";

        newAlliance.memberCivilizationIndices.push_back (initializerCivilization);
        newAlliance.memberCivilizationIndices.push_back (targetCivilizationIndex);

        gameVariables.alliances.push_back (newAlliance);

    }

}

void AllianceEvent::trigger_ai (GameVariables &gameVariables) {

    int Choice = 0;

    if (gameVariables.Civilizations[targetCivilizationIndex].relationsWithOtherCivilizations[initializerCivilization] >= -10) {

        Choice = choiceResponseCodes::YES;

    } else {

        Choice = choiceResponseCodes::NO;

    }

    this->responseLogic (Choice, gameVariables);

}
