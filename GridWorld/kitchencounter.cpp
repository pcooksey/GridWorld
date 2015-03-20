#include "kitchencounter.h"

KitchenCounter::KitchenCounter(const int &x, const int &y, const int &id, World* world)
:Object(x, y, id, world)
{
    type = ObjectBody::INANIMATE;
    load_image("images/cooking.bmp");
}

KitchenCounter::~KitchenCounter()
{
    //dtor
}
