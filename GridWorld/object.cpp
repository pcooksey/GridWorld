#include "object.h"
#include <cmath>

Object::Object(const int &x, const int &y, World* world)
    :x(x),y(y),world(world)
{
    type = ObjectBody::ALIVE;
}

Object::~Object()
{
    //dtor
}

void Object::setColor(int red, int blue, int green, int alpha)
{
    color.r = red;
    color.b = blue;
    color.g = green;
    color.a = alpha;
}

bool Object::move(int x, int y)
{
    //First make sure that the object isn't trying to move more than one square
    if(type==ObjectBody::INANIMATE ||
       fdim(this->x,x)>1 || fdim(this->y,y)>1 ||
       (fdim(this->x,x)==1 && fdim(this->y,y)==1))
        return false;
    if(this->x==x && this->y==y)
        return true;
    return world->move((this), x, y);
}
