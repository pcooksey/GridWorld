#include "conveyorbelt.h"

ConveyorBelt::ConveyorBelt(const int &id, World* world)
 :id(id), world(world)
{

}

ConveyorBelt::~ConveyorBelt()
{
    for(std::vector<Conveyor*>::iterator it=belt.begin(); it!=belt.end(); it++)
    {
        delete (*it);
    }
    belt.clear();
}

void ConveyorBelt::start()
{
    belt.push_back(new Conveyor(1,11,id,world));
    belt.push_back(new Conveyor(2,11,id,world));
    belt.push_back(new Conveyor(3,11,id,world));
    belt.push_back(new Conveyor(4,11,id,world));
    belt.push_back(new Conveyor(5,11,id,world));
    belt.push_back(new Conveyor(5,12,id,world));
    belt.push_back(new Conveyor(5,13,id,world));
    belt.push_back(new Conveyor(5,14,id,world));
    for(std::vector<Conveyor*>::iterator it=belt.begin(); it!=belt.end(); it++)
    {
        world->addObject((*it));
    }
}

void ConveyorBelt::execute()
{
    for(std::vector<Conveyor*>::iterator it=belt.begin(); it!=belt.end(); it++)
    {
        (*it)->execute();
    }
}
