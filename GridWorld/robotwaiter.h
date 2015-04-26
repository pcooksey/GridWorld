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
        enum Action {
            Waiting,
            GetOrder
        };

        RobotWaiter(const int &x, const int &y, const int &id, World* world);
        virtual ~RobotWaiter();

        void setAction(Action act);
        void addCustomer(Customer* customer);
        void setCommandControl(bool value)
                { commandControlled=value; };
        const std::vector<Customer*>* getAssignedCustomers()
                { return &customers; };
    protected:
    private:
        void execute();
        Customer* getClosestCustomer(std::vector<Customer*>* customers);
        Customer* findCustomer(const int& x, const int& y);

        std::vector<Customer*> customers;

        //Is the robot controller by the command center
        bool commandControlled;

        GridSearch::Node birthPlace;
        GridSearch::Path path;
        Rules defaultRules;
        RobotWaiterRules rules;
        Action action;
        World* world;
        Cafe* cafe;
};

#endif // ROBOTWAITER_H
