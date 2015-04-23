#ifndef CONVEYORBELT_H
#define CONVEYORBELT_H

#include <vector>
#include <algorithm>
#include <iterator>

#include "cafeobjects.h"
#include "conveyor.h"
#include "hamburger.h"

class ConveyorBelt
{
    public:
        ConveyorBelt(const int &id, World* world);
        virtual ~ConveyorBelt();

        /** Starts the conveyor belt by creating the objects */
        void start();
        /** Moves the food down the conveyor belt */
        void execute();
        /** Adds food to the conveyor belt */
        bool addFood(int food);
        /** Removes food from front of conveyor belt */
        int removeFood();
    protected:
    private:
        void rotateFood();
        void clearFood();

        std::vector<int> food;
        std::vector<Conveyor*> belt;
        std::vector<Object*> objects;
        int id;
        World* world;
};

#endif // CONVEYORBELT_H
