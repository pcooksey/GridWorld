#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>

#include "object.h"
#include "gridsearch.h"
#include "cafeobjects.h"

using namespace WorldObjects;

class CustomerRules: public Rules
{
    public:
        CustomerRules() { };
        virtual ~CustomerRules() {};
        bool check(GridSearch* searcher, int id);
};

class Customer : public Object
{
    public:
        Customer(const int &x, const int &y, const int &id, int identifier, World* world);
        virtual ~Customer();
        int askForOrder();
        bool receiveFood(int food);

        int getIdentifer() { return identifer; };

        bool readyToLeave;
    protected:
    private:
        void execute();

        int identifer;
        int orderRandomTime;
        GridSearch::Path path;
        CustomerRules rules;
        World* world;
};

#endif // CUSTOMER_H
