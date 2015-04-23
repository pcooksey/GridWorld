#include "robotserver.h"

RobotServer::RobotServer(const int &x, const int &y, const int &id, World* world)
  :Object(x, y, id, world), world(world)
{
    load_image("images/robotserver.bmp");
    cafe = static_cast<Cafe*>(world);
}

RobotServer::~RobotServer()
{
    //dtor
}

void RobotServer::execute()
{

}
