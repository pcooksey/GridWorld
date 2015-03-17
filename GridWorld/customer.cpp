#include "customer.h"

Customer::Customer(const int &x, const int &y, World* world)
:Object(x, y, world), world(world)
{
    type = ObjectBody::ALIVE;
}

Customer::~Customer()
{
    //dtor
}

void Customer::execute()
{
    move(getx()-1,gety());
}
