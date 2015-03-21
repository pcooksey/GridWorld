#ifndef KITCHEN_H
#define KITCHEN_H

#include <vector>
#include <algorithm>
#include <iterator>

#include "cafeobjects.h"
#include "kitchencounter.h"
#include "hamburger.h"

class Kitchen
{
    public:
        Kitchen(const int &id, World* world);
        virtual ~Kitchen();
        /** Starts the kitchen by creating the counter */
        void start();
        /** Returns the id of a finished cooked food */
        int execute();
        /** Adds food to counter if there is space otherwise returns false */
        bool addFood(int food);
    protected:
    private:

        std::vector<int> foodTimes;
        std::vector<int> food;
        std::vector<KitchenCounter*> counters;
        std::vector<Object*> objects;

        int id;
        World* world;
};

#endif // KITCHEN_H
