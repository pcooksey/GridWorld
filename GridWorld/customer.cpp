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
    //std::cout<<((rand()%2-1?-1:1)*(rand()%2-1));
    move(getx()+((rand()%2-1?-1:1)*(rand()%2-1)),gety()+((rand()%2-1?-1:1)*(rand()%2-1)));
}
