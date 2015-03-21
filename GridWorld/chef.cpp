#include "chef.h"

Chef::Chef(const int &x, const int &y, const int &id, World* world)
 :Object(x, y, id, world), world(world)
{
    load_image("images/chef2.bmp");
}

Chef::~Chef()
{
    //dtor
}

void Chef::execute()
{
    move(getx()+((rand()%2-1?-1:1)*(rand()%2-1)),gety()+((rand()%2-1?-1:1)*(rand()%2-1)));
}
