#include "commandcenter.h"

CommandCenter::CommandCenter(World* world)
: world(world)
{
    operationMethod = ByOrder;
    cafe = static_cast<Cafe*>(world);
    for(std::vector<RobotWaiter*>::iterator it=cafe->robotwaiters.begin(); it!=cafe->robotwaiters.end(); it++)
    {
        (*it)->setCommandControl(true);
    }
    for(std::vector<RobotServer*>::iterator it=cafe->robotservers.begin(); it!=cafe->robotservers.end(); it++)
    {
        (*it)->setCommandControl(true);
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
