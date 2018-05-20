#ifndef LOANEVENT_H
#define LOANEVENT_H
#include "Event.h"
#include "Loan.h"

class LoanEvent : public Event
{
    public:
        LoanEvent();
        virtual ~LoanEvent();

        int LoanID;

        void trigger (std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans);
        void trigger_ai (std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans);
        void responseLogic (int Choice, std::vector<Civilization> &Civilizations, std::vector<Loan> &Loans);
    protected:
    private:
};

#endif // LOANEVENT_H
