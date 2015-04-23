#include "robotarm.h"

RobotArm::RobotArm(const int &x, const int &y, const int &id, World* world)
 :Object(x, y, id, world), world(world)
{
    load_image("images/robotarm.bmp");
    cafe = static_cast<Cafe*>(world);
}

RobotArm::~RobotArm()
{
    //dtor
}

void RobotArm::execute()
{

}

int RobotArm::grabFood()
{
    return cafe->belt.removeFood();
}
