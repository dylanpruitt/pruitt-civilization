#include "LoanEvent.h"
#include "sharedMethods.h"
#include <iostream>

LoanEvent::LoanEvent()
{
    //ctor
}

LoanEvent::~LoanEvent()
{
    //dtor
}

enum choiceResponseCodes : int {

    YES = 1,
    NO = 2,

};

void LoanEvent::responseLogic (int Choice, std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans) {

        if (Choice == choiceResponseCodes::YES) {

            Loans[LoanID].isAccepted = true; std::cout << Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold  << std::endl;

            Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold -= Loans[LoanID].amountDue;std::cout << Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold  << std::endl;

            Civilizations[Loans[LoanID].debtorCivilizationIndex].Gold += Loans[LoanID].amountDue;

        } else {

            Loans.erase (Loans.begin() + LoanID);

        }

}

void LoanEvent::trigger (std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans) {

    std::cout << "\n\n[====== " << EventName << " ======]" << std::endl;
    std::cout << "\n" << EventMessage << std::endl;

    for (unsigned int i = 1; i <= ResponseChoices.size(); i++) {

        std::cout << "[" << i << "] " << ResponseChoices[i-1] << std::endl;

    }

    int Choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

    responseLogic(Choice, Civilizations, Loans);

}


void LoanEvent::trigger_ai (std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans) {

    int Choice = 0;

    if (Civilizations[targetCivilizationIndex].relationsWithOtherCivilizations[initializerCivilization] >= -10) {

        Choice = choiceResponseCodes::YES;

    } else {

         Choice = choiceResponseCodes::NO;

    }

    responseLogic(Choice, Civilizations, Loans);

}
