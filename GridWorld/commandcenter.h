#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "cafe.h"

using namespace WorldObjects;

class Cafe;

class CommandCenter
{
    public:
        enum Method {
            ByOrder,
            ByRoomPlacement
        };
        CommandCenter(World* world);
        virtual ~CommandCenter();
        void execute();
    protected:
    private:
        bool assignedWaiter(Customer* customer);
        bool assignedServer(Customer* customer);

        int waiterToAssignNext;
        int serverToAssignNext;
        Method operationMethod;
        World* world;
        Cafe* cafe;
};

#endif // COMMANDCENTER_H
