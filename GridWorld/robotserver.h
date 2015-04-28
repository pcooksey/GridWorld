#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H

#include "object.h"
#include "cafe.h"
#include "gridsearch.h"
#include <assert.h>

using namespace WorldObjects;

class Cafe;

class RobotServerRules: public Rules
{
    public:
        RobotServerRules() {};
        virtual ~RobotServerRules() {};
        bool check(GridSearch* searcher, int id);
};

class RobotServer : public Object
{
    public:
        enum Action {
            Nothing,
            GetFood,
            ServerFood
        };

        RobotServer(const int &x, const int &y, const int &id, World* world);
        virtual ~RobotServer();

        void setAction(Action act);
        void addCustomer(Customer* customer);
        void setCommandControl(bool value)
                { commandControlled=value; };
        const std::vector<Customer*>* getAssignedCustomers()
                { return &customers; };
    protected:
    private:
        void execute();

        int timeHolding;

        std::vector<int> food;
        std::vector<Customer*> customers;

        bool commandControlled;

        GridSearch::Node birthPlace;
        GridSearch::Path path;
        Rules defaultRules;
        RobotServerRules rules;
        Action action;
        World* world;
        Cafe* cafe;
};

#endif // ROBOTSERVER_H
