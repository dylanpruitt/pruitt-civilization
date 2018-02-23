#ifndef EVENT_H
#define EVENT_H
#include "GameVariables.h"

#include <string>

class Event
{
    public:
        Event();
        virtual ~Event();

        std::string EventName;
        std::string EventMessage;

        std::vector<std::string> ResponseChoices;

        bool test_canBeTriggered = true;
        bool hasBeenTriggered = false;

        int targetCivilizationIndex;

        std::string eventType;

        int initializerCivilization; /// Index of civ that triggers the event (ex. alliance)

        void listen (int civilizationIndex, GameVariables &gameVariables);

    protected:
    private:

        void trigger (GameVariables &gameVariables);

        void trigger_ai (GameVariables &gameVariables);

        void responseLogic (int Choice, GameVariables &gameVariables);
};

#endif // EVENT_H
