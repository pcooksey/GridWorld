#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include "cafe.h"

using namespace WorldObjects;

class Cafe;

class CommandCenter
{
    public:
        CommandCenter(World* world);
        virtual ~CommandCenter();
        void execute();
    protected:
    private:

        World* world;
        Cafe* cafe;
};

#endif // COMMANDCENTER_H
