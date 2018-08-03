#ifndef EVENT_H
#define EVENT_H
#include "Civilization.h"
#include "GameVariables.h"
#include "sharedMethods.h"

#include <string>
#include <iostream>

class Event
{
    public:
        Event();

        virtual ~Event();

        std::string EventName;
        std::string EventMessage;

        std::vector<std::string> ResponseChoices;

        bool hasBeenTriggered = false;

        int targetCivilizationIndex;

        int initializerCivilization; /// Index of civ that triggers the event (ex. alliance)

        void listen (int civilizationIndex, GameVariables &gameVariables);

    protected:


        enum choiceResponseCodes : int {

            YES = 1,
            NO = 2,

        };

    private:

        virtual void trigger (GameVariables &gameVariables);

        virtual void trigger_ai (GameVariables &gameVariables);

        virtual void responseLogic (int Choice, GameVariables &gameVariables);

};

#endif // EVENT_H
