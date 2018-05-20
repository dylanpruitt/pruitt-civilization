#ifndef LOAN_H
#define LOAN_H


class Loan
{
    public:
        Loan() {};
        virtual ~Loan() { };

        int amountDue = 25;

        int amountPaid;

        int creditorCivilizationIndex;
        int debtorCivilizationIndex;

        bool isAccepted = false;

    protected:
    private:
};

#endif // LOAN_H
