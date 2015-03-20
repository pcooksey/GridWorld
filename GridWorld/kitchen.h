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
        void start();
        void execute();
        bool addFood(int food);
    protected:
    private:

        std::vector<int> food;
        std::vector<KitchenCounter*> counters;
        std::vector<Object*> objects;

        int id;
        World* world;
};

#endif // KITCHEN_H
