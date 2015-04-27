#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "cafe.h"
#include "gridsearch.h"

using namespace WorldObjects;

class Cafe;

//Rules class is used very differently in the command class just checking for tables
class CommandCenterRules: public Rules
{
    public:
        CommandCenterRules() {};
        virtual ~CommandCenterRules() {};
        bool check(GridSearch* searcher, int id);
};

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
        bool atTable(Customer* customer);

        int waiterToAssignNext;
        int serverToAssignNext;
        CommandCenterRules rules;
        Method operationMethod;
        World* world;
        Cafe* cafe;
};

#endif // COMMANDCENTER_H
