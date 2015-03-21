#ifndef CHEF_H
#define CHEF_H

#include "object.h"


class Chef : public Object
{
    public:
        Chef(const int &x, const int &y, const int &id, World* world);
        virtual ~Chef();
    protected:
    private:
        void execute();

        World* world;
};

#endif // CHEF_H
