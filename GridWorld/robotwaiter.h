#ifndef ROBOTWAITER_H
#define ROBOTWAITER_H

#include "object.h"
#include "gridsearch.h"
#include "cafe.h"

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

        std::vector<Customer*> customers;
        std::vector<Customer*> visited;

        GridSearch::Path path;
        RobotWaiterRules rules;
        World* world;
        Cafe* cafe;
};

#endif // ROBOTWAITER_H
