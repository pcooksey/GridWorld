#ifndef ROBOTARM_H
#define ROBOTARM_H

#include "object.h"
#include "cafe.h"

class Cafe;

class RobotArm : public Object
{
    public:
        RobotArm(const int &x, const int &y, const int &id, World* world);
        virtual ~RobotArm();
        void execute();

    protected:

    private:

        World* world;
        Cafe* cafe;
};

#endif // ROBOTARM_H
