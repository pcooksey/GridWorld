#include "customer.h"

Customer::Customer(const int &x, const int &y, World* world)
:Object(x, y, world), world(world)
{

}

Customer::~Customer()
{
    //dtor
}

void Customer::execute()
{
    move(getx()+((rand()%2-1?-1:1)*(rand()%2-1)),gety()+((rand()%2-1?-1:1)*(rand()%2-1)));
}
