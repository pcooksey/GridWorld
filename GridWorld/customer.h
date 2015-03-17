#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "object.h"


class Customer : public Object
{
    public:
        Customer(const int &x, const int &y, World* world);
        virtual ~Customer();
    protected:
    private:
        void execute();

        World* world;
};

#endif // CUSTOMER_H
