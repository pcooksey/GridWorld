#ifndef KITCHEN_H
#define KITCHEN_H

#include <vector>
#include <algorithm>
#include <iterator>

#include "cafe.h"
#include "cafeobjects.h"
#include "kitchencounter.h"
#include "hamburger.h"

class Cafe;

class Kitchen
{
    public:
        Kitchen(const int &id, World* world);
        virtual ~Kitchen();
        /** Starts the kitchen by creating the counter */
        void start();
        /** Cooks the food and adds to conveyor belt when done */
        void execute();
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
        Cafe* cafe;
};

#endif // KITCHEN_H
