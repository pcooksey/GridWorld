#ifndef CAFE_H
#define CAFE_H

#include <algorithm>
#include <iostream>

#include "world.h"
#include "cafeobjects.h"
#include "customer.h"
#include "chef.h"
#include "robotwaiter.h"
#include "conveyorbelt.h"
#include "kitchen.h"
#include "robotarm.h"

class RobotWaiter;
class Kitchen;
class RobotArm;

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
        void createRobotWaiter();
        void createRobotArm();

    private:

        const Map& gridMap;
        ConveyorBelt belt;
        Kitchen* kitchen;
        Chef* chef;
        RobotArm* robotArm;
        std::vector<int> orders;
        std::vector<Node > doorways;
        std::vector<Customer*> customers;
        std::vector<RobotWaiter*> robotwaiters;

        friend class RobotWaiter;
        friend class Kitchen;
        friend class RobotArm;
};

#endif // CAFE_H
