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
                if(cafe->belt.addFood(food[iteratorNum]) || (*it)>=15)
                {
                    Object* object = objects[iteratorNum];
                    world->removeObject(object);
                    objects.erase(std::find(objects.begin(),objects.end(),object));
                    delete object;
                    if((*it)>=15)
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
    std::vector<int>::iterator it = std::find(food.begin(), food.end(), 0);
    if(it!=food.end())
    {
        KitchenCounter* kit = counters[it-food.begin()];
        (*it)=foodNum;
        if(foodNum==WorldObjects::HAMBURGER)
        {
            objects.push_back(new Hamburger(kit->getx(),kit->gety(),foodNum, world));
            world->addObject(objects.back());
        } else {

        }
        return true;
    }
    return false;
}
