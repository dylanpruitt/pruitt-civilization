#ifndef ALLIANCEEVENT_H
#define ALLIANCEEVENT_H
#include "Alliance.h"
#include "Event.h"
#include <iostream>

class AllianceEvent : public Event
{
    public:

        AllianceEvent();

        virtual ~AllianceEvent();

    protected:
    private:

        virtual void trigger (GameVariables &gameVariables);

        virtual void responseLogic (int Choice, GameVariables &gameVariables);

        virtual void trigger_ai (GameVariables &gameVariables);

};

#endif // ALLIANCEEVENT_H
