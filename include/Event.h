#ifndef EVENT_H
#define EVENT_H
#include "Civilization.h"
#include "Alliance.h"

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

        void listen (int civilizationIndex, std::vector<Civilization> &Civilizations);

    protected:
    private:

        virtual void trigger (std::vector<Civilization> &Civilizations);

        virtual void trigger_ai (std::vector<Civilization> &Civilizations);

        virtual void responseLogic (int Choice, std::vector<Civilization> &Civilizations);

};

#endif // EVENT_H
