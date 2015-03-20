#ifndef KITCHENCOUNTER_H
#define KITCHENCOUNTER_H

#include "object.h"


class KitchenCounter : public Object
{
    public:
        KitchenCounter(const int &x, const int &y, const int &id, World* world);
        virtual ~KitchenCounter();
        void execute() {};
    protected:
    private:
};

#endif // KITCHENCOUNTER_H
