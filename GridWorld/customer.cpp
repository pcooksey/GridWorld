#include "customer.h"

Customer::Customer(const int &x, const int &y, const int &id, World* world)
:Object(x, y, id, world), world(world)
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
