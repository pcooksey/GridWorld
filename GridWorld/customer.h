#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>

#include "object.h"
#include "gridsearch.h"


class Customer : public Object
{
    public:
        Customer(const int &x, const int &y, const int &id, World* world);
        virtual ~Customer();
    protected:
    private:
        void execute();

        GridSearch::Path path;
        Rules rules;
        World* world;
};

#endif // CUSTOMER_H
