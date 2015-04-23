#include "robotwaiter.h"

bool RobotWaiterRules::check(GridSearch* searcher, int id)
{
    if(getGoal(searcher)==GridSearch::NullNode) {
        if(id==OPENSPACE || id==DOORWAY)
            return true;
        if(id==CUSTOMER)
        {
            IDs nextToRobot = getBranchesIDs(searcher, getCurrent(searcher));
            // Check if customer is next to TABLE. This however does not always work since the human
            // can walk pass a table.
            IDs::iterator it = std::find(nextToRobot.begin(),nextToRobot.end(), TABLE);
            if(it!=nextToRobot.end())
                return true;
            else
                return false;
        }
    } else {
        if(id==OPENSPACE || id==DOORWAY)
        {
            if(getGoal(searcher)==getChecking(searcher))
            {
                IDs nextToRobot = getBranchesIDs(searcher, getCurrent(searcher));
                IDs::iterator it = std::find(nextToRobot.begin(),nextToRobot.end(), TABLE);
                if(it!=nextToRobot.end())
                    return true;
                else
                    return false;
            }
            return true;
        }
        if(id==CUSTOMER)
        {
            if(getGoal(searcher)==getChecking(searcher))
            {
                IDs nextToRobot = getBranchesIDs(searcher, getChecking(searcher));
                IDs::iterator it = std::find(nextToRobot.begin(),nextToRobot.end(), TABLE);
                if(it!=nextToRobot.end())
                    return true;
                else
                    return false;
            }
        }
    }
    return false;
}

RobotWaiter::RobotWaiter(const int &x, const int &y, const int &id, World* world)
  :Object(x, y, id, world), rules(), world(world)
{
    load_image("images/robotwaiter.bmp");
    cafe = static_cast<Cafe*>(world);
}

RobotWaiter::~RobotWaiter()
{
    //dtor
}

/**

    TODO RobotWaiter need to only server each customer once and then
    go to the new customer. It needs to dynamically search for the customer
    so it doesn't waste time going to where the customer used to be.
    This is a difficult problem. Even more difficult will be devising a way for
    multiple of these robots to coordinate with each other.

    It now find the closest customer not visited already and orders from them. However,
    this does not take into account that a path to the customer might not exist and
    does not search again. There are issues with this naive way of doing it but that
    is the point to compare to our later results.

*/

void RobotWaiter::execute()
{
    if(path.empty())
    {
        customers = &cafe->customers;
        GridSearch searcher(world->getWorldGrid(),&rules);
        Customer* customer = getClosestCustomer();
        if(customer!=NULL)
        {
            GridSearch::Node customerNode(customer->getx(),customer->gety());
            path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
        }
    } else {
        GridSearch::Node node = path.front();
        const Map::MultiArray& worldMap = world->getWorldGrid().getGrid();
        if(path.size()<=2 && worldMap[node.first][node.second]==WorldObjects::CUSTOMER)
        {
            Customer* customer = findCustomer(node.first, node.second);
            if(customer!=NULL)
            {
                int order = customer->askForOrder();
                if(order!=-1)
                {
                    cafe->orders.push_back(order);
                    path.clear();
                    visited.push_back(customer);
                }
            } else {
                // Made a mistake! There was no customer
                path.clear();
            }
        }
        else if(move(node.first,node.second))
        {
            path.erase(path.begin());
        }
        else
        {
            path.clear();
        }
    }
}

Customer* RobotWaiter::getClosestCustomer()
{
    Customer* nully = NULL;
    int distance(Screen::SCREEN_HEIGHT), temp(0);
    int x = getx(), y = gety(), x2(0), y2(0);
    for(std::vector<Customer*>::iterator it = customers->begin(); it!=customers->end(); it++)
    {
        if(std::find(visited.begin(),visited.end(),(*it))==visited.end())
        {
            x2 = (*it)->getx();
            y2 = (*it)->gety();
            int temp = sqrt(pow(x2-x,2)-pow(y2-y,2));
            if(temp<distance)
            {
                nully = (*it);
                distance = temp;
            }
        }
    }
    return nully;
}

Customer* RobotWaiter::findCustomer(const int& x, const int& y)
{
    Customer* nully = NULL;
    for(std::vector<Customer*>::iterator it = customers->begin(); it!=customers->end(); it++)
    {
        if((*it)->getx()==x && (*it)->gety()==y)
            return (*it);
    }
    return nully;
}
