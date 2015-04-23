#include "commandcenter.h"

CommandCenter::CommandCenter(World* world)
: world(world)
{
    cafe = static_cast<Cafe*>(world);
}

CommandCenter::~CommandCenter()
{
    //dtor
}

void CommandCenter::execute()
{

}
