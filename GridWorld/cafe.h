#ifndef CAFE_H
#define CAFE_H

#include <algorithm>
#include <iostream>

#include "world.h"
#include "cafeobjects.h"
#include "customer.h"
#include "robotwaiter.h"
#include "conveyorbelt.h"
#include "kitchen.h"

class RobotWaiter;

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

    private:

        const Map& gridMap;
        ConveyorBelt belt;
        Kitchen kitchen;
        std::vector<Node > doorways;
        std::vector<Customer*> customers;
        std::vector<RobotWaiter*> robotwaiters;

        friend class RobotWaiter;
};

#endif // CAFE_H
