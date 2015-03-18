#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>

#include "object.h"
#include "gridsearch.h"

/** Customer class will need to make a custom Rules
    class in order to search and find a table.
    Look at the example Rules class and ID == table
    should be set. The customer however will not walk on
    table since that is stopped by the world class */

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
        Customer(const int &x, const int &y, const int &id, World* world);
        virtual ~Customer();
    protected:
    private:
        void execute();

        GridSearch::Path path;
        CustomerRules rules;
        World* world;
};

#endif // CUSTOMER_H
