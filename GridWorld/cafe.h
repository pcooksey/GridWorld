#ifndef CAFE_H
#define CAFE_H

#include <algorithm>
#include <iostream>

#include "world.h"
#include "customer.h"


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

    private:

        const Map& gridMap;
        std::vector<Node > doorways;
        std::vector<Customer*> customers;
};

#endif // CAFE_H
