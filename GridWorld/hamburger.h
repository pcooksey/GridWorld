#ifndef HAMBURGER_H
#define HAMBURGER_H

#include "object.h"


class Hamburger : public Object
{
    public:
        Hamburger(const int &x, const int &y, const int &id, World* world);
        virtual ~Hamburger();
    protected:
    private:
        void execute() {};
};

#endif // HAMBURGER_H
