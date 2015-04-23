#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H

#include "object.h"
#include "cafe.h"

using namespace WorldObjects;

class Cafe;

class RobotServer : public Object
{
    public:
        RobotServer(const int &x, const int &y, const int &id, World* world);
        virtual ~RobotServer();
    protected:
    private:
        void execute();

        std::vector<int> food;

        World* world;
        Cafe* cafe;
};

#endif // ROBOTSERVER_H
