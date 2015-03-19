#ifndef CONVEYORBELT_H
#define CONVEYORBELT_H

#include <vector>

#include "conveyor.h"

class ConveyorBelt
{
    public:
        ConveyorBelt(const int &id, World* world);
        virtual ~ConveyorBelt();

        void start();
        void execute();
    protected:
    private:

        std::vector<Conveyor*> belt;
        int id;
        World* world;
};

#endif // CONVEYORBELT_H
