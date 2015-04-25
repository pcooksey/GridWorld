#ifndef CAFE_H
#define CAFE_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "cafeobjects.h"
#include "customer.h"
#include "chef.h"
#include "robotwaiter.h"
#include "robotserver.h"
#include "conveyorbelt.h"
#include "kitchen.h"
#include "robotarm.h"
#include "commandcenter.h"

class RobotWaiter;
class RobotServer;
class Kitchen;
class RobotArm;
class CommandCenter;

class Cafe : public World
{
    public:
        typedef std::pair<int, int> Node;
        Cafe(std::string fileName);
        virtual ~Cafe();
        bool successful();
        void start();
    protected:
        void execute();
        void findDoors();
        Node getADoor();
        void createCustomer();
        void createRobotWaiter(int x, int y);
        void createRobotServer(int x, int y);
        void createRobotArm();

    private:

        unsigned int time;

        int identifier;

        const Map& gridMap;
        ConveyorBelt belt;
        Kitchen* kitchen;
        Chef* chef;
        RobotArm* robotArm;
        CommandCenter* commandCenter;
        std::vector<int> orders;
        std::vector<std::pair<Customer*, int> > orderMap;
        std::vector<Node > doorways;
        std::vector<Customer*> customers;
        std::vector<int> visited;
        std::map<int,int> customerTimes;
        std::vector<RobotWaiter*> robotwaiters;
        std::vector<RobotServer*> robotservers;

        friend class RobotWaiter;
        friend class RobotServer;
        friend class Kitchen;
        friend class RobotArm;
        friend class CommandCenter;
};

#endif // CAFE_H
