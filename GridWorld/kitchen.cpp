#include "kitchen.h"

Kitchen::Kitchen(const int &id, World* world)
:id(id), world(world)
{
    cafe = static_cast<Cafe*>(world);
}

Kitchen::~Kitchen()
{
    for(std::vector<KitchenCounter*>::iterator it=counters.begin(); it!=counters.end(); it++)
    {
        delete (*it);
    }
    counters.clear();
}

void Kitchen::start()
{
    counters.push_back(new KitchenCounter(1,12,id,world));
    counters.push_back(new KitchenCounter(1,13,id,world));
    counters.push_back(new KitchenCounter(1,14,id,world));
    counters.push_back(new KitchenCounter(1,15,id,world));
    for(std::vector<KitchenCounter*>::iterator it=counters.begin(); it!=counters.end(); it++)
    {
        world->addObject((*it));
    }
    food.resize(counters.size(),0);
    foodTimes.resize(counters.size(),0);
    objects.resize(counters.size(),NULL);
}

void Kitchen::execute()
{
    int iteratorNum = 0;
    for(std::vector<int>::iterator it=foodTimes.begin(); it!=foodTimes.end(); it++, iteratorNum++)
    {
        if(food[iteratorNum]!=0)
        {
            (*it)++;
            if((*it)>=10)
            {
                bool foodAdded = cafe->belt.addFood(food[iteratorNum]);
                if(foodAdded || (*it)>=15)
                {
                    Object* object = objects[iteratorNum];
                    assert(object!=NULL);
                    world->removeObject(object);
                    objects[iteratorNum] = NULL;
                    delete object;
                    if(!foodAdded && (*it)>=15)
                    {
                        cafe->orders.push_back(food[iteratorNum]);
                    }
                    food[iteratorNum] = 0;
                    (*it)=0;
                }
            }
        }
    }
}

bool Kitchen::addFood(int foodNum)
{
    assert(foodNum>0);
    std::vector<int>::iterator it = std::find(food.begin(), food.end(), 0);
    if(it!=food.end())
    {
        if(foodNum==WorldObjects::HAMBURGER)
        {
            KitchenCounter* kit = counters[it-food.begin()];
            (*it)=foodNum;
            objects[it-food.begin()] = new Hamburger(kit->getx(),kit->gety(),foodNum, world);
            world->addObject(objects[it-food.begin()]);
            return true;
        }
    }
    return false;
}
