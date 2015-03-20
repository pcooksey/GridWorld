#include "hamburger.h"

Hamburger::Hamburger(const int &x, const int &y, const int &id, World* world)
 :Object(x, y, id, world)
{
    load_image("images/hamburger.bmp");
}

Hamburger::~Hamburger()
{
    //dtor
}
