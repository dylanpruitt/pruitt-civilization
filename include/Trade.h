#ifndef TRADE_H
#define TRADE_H
#include "City.h"
#include "Resource.h"

class Trade
{
    public:
        Trade() {}
        virtual ~Trade() {}

        int traderIndex = 0;
        int recipientIndex = 0;

        int goldSumFromTrader = 0;
        int goldSumFromRecipient = 0;

        int GPTFromTrader = 0;
        int GPTFromRecipient = 0;

        std::vector<Resource> resourcesFromTrader;
        std::vector<Resource> resourcesFromRecipient;

    protected:
    private:
};

#endif // TRADE_H
