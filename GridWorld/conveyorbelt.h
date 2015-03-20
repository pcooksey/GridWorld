#ifndef CONVEYORBELT_H
#define CONVEYORBELT_H

#include <vector>
#include <algorithm>
#include <iterator>

#include "conveyor.h"
#include "hamburger.h"

class ConveyorBelt
{
    public:
        ConveyorBelt(const int &id, World* world);
        virtual ~ConveyorBelt();

        void start();
        void execute();
        bool addFood(int food);
    protected:
    private:
        void rotateFood();
        void clearFood();

        std::vector<int> food;
        std::vector<Conveyor*> belt;
        std::vector<Hamburger*> hamburgers;
        int id;
        World* world;
};

#endif // CONVEYORBELT_H
