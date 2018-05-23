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

        std::cout << Choice << std::endl;

        if (Choice == choiceResponseCodes::YES) {

            Loans[LoanID].isAccepted = true; std::cout << Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold  << std::endl;

            Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold -= Loans[LoanID].amountDue;std::cout << Civilizations[Loans[LoanID].creditorCivilizationIndex].Gold  << std::endl;

            Civilizations[Loans[LoanID].debtorCivilizationIndex].Gold += Loans[LoanID].amountDue;

        } else {

            std::cout << "Is this even being called??" << std::endl;

            Loans.erase (Loans.begin() + LoanID);

        }

}

void LoanEvent::trigger (std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans) {

    std::cout << "\n\n[====== " << EventName << " ======]" << std::endl;
    std::cout << "\n" << EventMessage << std::endl;

    for (unsigned int i = 1; i <= ResponseChoices.size(); i++) {

        std::cout << ResponseChoices.size() << std::endl;


        std::cout << "[" << i << "] " << ResponseChoices[i-1] << std::endl;

    }


    if (ResponseChoices.size() >= 1) {

        int choice = sharedMethods::bindIntegerInputToRange(1, ResponseChoices.size(), 1);

        responseLogic(choice, Civilizations, Loans);

    }

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
