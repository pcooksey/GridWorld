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
    food.resize(belt.size(),0);
}

void ConveyorBelt::execute()
{
    clearFood();
    rotateFood();
    int foodNum = 0;
    for(std::vector<Conveyor*>::iterator it=belt.begin(); it!=belt.end(); it++)
    {
        (*it)->execute();
        if(food[foodNum]==11)//Hamburger need to figure out way to get global namespace to work
        {
            hamburgers.push_back(new Hamburger((*it)->getx(),(*it)->gety(),11, world));
            world->addObject(hamburgers.back());
        } else {

        }
        foodNum++;
    }
}

bool ConveyorBelt::addFood(int foodNum)
{
    if(food.front()==0)
    {
        rotateFood();
        food[0]=foodNum;
        return true;
    }
    return false;
}

void ConveyorBelt::clearFood()
{

    for(std::vector<Hamburger*>::iterator it=hamburgers.begin(); it!=hamburgers.end(); it++)
    {
        world->removeObject((*it));
        delete (*it);
    }
    hamburgers.clear();
}

void ConveyorBelt::rotateFood()
{
    std::vector<int>::reverse_iterator next;
    for(std::vector<int>::reverse_iterator it=food.rbegin(); it!=food.rend(); it++)
    {
        next = it;
        next++;
        if((*it)==0)
        {
            (*it)=(*next);
            (*next)=0;
        }
    }
}
