#ifndef ROBOTWAITER_H
#define ROBOTWAITER_H

#include <math.h>
#include <fstream>

#include "object.h"
#include "gridsearch.h"
#include "cafe.h"

/**
 There is a bug where the RobotWaiter asks a customer not at a seat and
 I think adds it to the visited which messes up everything!!! AGHH!
*/

using namespace WorldObjects;

class Cafe;

class RobotWaiterRules: public Rules
{
    public:
        RobotWaiterRules() {};
        virtual ~RobotWaiterRules() {};
        bool check(GridSearch* searcher, int id);
};

class RobotWaiter : public Object
{
    public:
        RobotWaiter(const int &x, const int &y, const int &id, World* world);
        virtual ~RobotWaiter();
    protected:
    private:
        void execute();
        Customer* getClosestCustomer();
        Customer* findCustomer(const int& x, const int& y);

        std::vector<Customer*>* customers;
        std::vector<int> visited;

        GridSearch::Path path;
        RobotWaiterRules rules;
        World* world;
        Cafe* cafe;
};

#endif // ROBOTWAITER_H
