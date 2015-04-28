#include "commandcenter.h"

bool CommandCenterRules::check(GridSearch* searcher, int id)
{
    if(id==TABLE)
        return true;
    return false;
}

CommandCenter::CommandCenter(World* world, bool turnedOn)
: world(world), turnedOn(turnedOn)
{
    operationMethod = ByOrder;
    cafe = static_cast<Cafe*>(world);
    if(turnedOn)
    {
        for(std::vector<RobotWaiter*>::iterator it=cafe->robotwaiters.begin(); it!=cafe->robotwaiters.end(); it++)
        {
            (*it)->setCommandControl(true);
        }
        for(std::vector<RobotServer*>::iterator it=cafe->robotservers.begin(); it!=cafe->robotservers.end(); it++)
        {
            (*it)->setCommandControl(true);
        }
    }
    waiterToAssignNext = 0;
    serverToAssignNext = 0;
}

CommandCenter::~CommandCenter()
{
    //dtor
}

void CommandCenter::execute()
{
    if(!turnedOn)
        return;
    switch(operationMethod)
    {
    case ByOrder:
        for(std::vector<Customer*>::iterator it=cafe->customers.begin(); it!=cafe->customers.end(); it++)
        {
            if(std::find(cafe->visited.begin(),cafe->visited.end(),(*it)->getIdentifer())==cafe->visited.end() &&
                !assignedWaiter((*it)))
            {
                cafe->robotwaiters[waiterToAssignNext]->addCustomer((*it));
                waiterToAssignNext++;
                if(waiterToAssignNext>=cafe->robotwaiters.size())
                    waiterToAssignNext = 0;
            }
        }

        for(std::vector<std::pair<Customer*, int> >::iterator it=cafe->orderMap.begin(); it!=cafe->orderMap.end(); it++)
        {
            if(!assignedServer((*it).first))
            {
                cafe->robotservers[serverToAssignNext]->addCustomer((*it).first);
                serverToAssignNext++;
                if(serverToAssignNext>=cafe->robotservers.size())
                    serverToAssignNext = 0;
            }
        }

        break;
    case ByRoomPlacement:
        //This one has to be designed for each amount of robot or figure out some general way of splitting the room
        //Right now splitting right down the middle of the room
        for(std::vector<Customer*>::iterator it=cafe->customers.begin(); it!=cafe->customers.end(); it++)
        {
            if(atTable((*it)) && std::find(cafe->visited.begin(),cafe->visited.end(),(*it)->getIdentifer())==cafe->visited.end() &&
                !assignedWaiter((*it)))
            {
                if((*it)->gety()>(world->getGrid().getSize().second/2))
                {
                    waiterToAssignNext = 1;
                } else {
                    waiterToAssignNext = 0;
                }
                cafe->robotwaiters[waiterToAssignNext]->addCustomer((*it));
            }
        }

        for(std::vector<std::pair<Customer*, int> >::iterator it=cafe->orderMap.begin(); it!=cafe->orderMap.end(); it++)
        {
            if(!assignedServer((*it).first))
            {
                if((*it).first->gety()>(world->getGrid().getSize().second/2))
                {
                    serverToAssignNext = 1;
                } else {
                    serverToAssignNext = 0;
                }
                cafe->robotservers[serverToAssignNext]->addCustomer((*it).first);
            }
        }
        break;
    }
}

bool CommandCenter::assignedWaiter(Customer* customer)
{
    const std::vector<Customer*>* customers;
    for(std::vector<RobotWaiter*>::iterator it=cafe->robotwaiters.begin(); it!=cafe->robotwaiters.end(); it++)
    {
        customers = (*it)->getAssignedCustomers();
        for(std::vector<Customer*>::const_iterator it=customers->begin(); it!=customers->end(); it++)
        {
            if((*it)==customer)
                return true;
        }
    }
    return false;
}

bool CommandCenter::assignedServer(Customer* customer)
{
    const std::vector<Customer*>* customers;
    for(std::vector<RobotServer*>::iterator it=cafe->robotservers.begin(); it!=cafe->robotservers.end(); it++)
    {
        customers = (*it)->getAssignedCustomers();
        for(std::vector<Customer*>::const_iterator it=customers->begin(); it!=customers->end(); it++)
        {
            if((*it)==customer)
                return true;
        }
    }
    return false;
}

bool CommandCenter::atTable(Customer* customer)
{
    GridSearch::Node node(customer->getx(),customer->gety());
    GridSearch searcher(world->getWorldGrid(),&rules);
    GridSearch::Path blocks = searcher.getBranches(node);
    if(blocks.size()>0)
        return true;
    return false;
}
