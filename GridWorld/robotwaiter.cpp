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
            if(it!=nextToRobot.end()){
                return true;
            } else {
                return false;
            }
        }
    } else {
        if(id==OPENSPACE || id==DOORWAY)
        {
            if(getGoal(searcher)==getChecking(searcher))
            {
                IDs nextToRobot = getBranchesIDs(searcher, getChecking(searcher));
                //IDs nextToRobot = getBranchesIDs(searcher, getCurrent(searcher));
                IDs::iterator it = std::find(nextToRobot.begin(),nextToRobot.end(), TABLE);
                if(it!=nextToRobot.end()){
                    return true;
                } else {
                    return false;
                }
            }
            return true;
        }
        if(id==CUSTOMER)
        {
            if(getGoal(searcher)==getChecking(searcher))
            {
                IDs nextToRobot = getBranchesIDs(searcher, getChecking(searcher));
                IDs::iterator it = std::find(nextToRobot.begin(),nextToRobot.end(), TABLE);
                if(it!=nextToRobot.end()){
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}

RobotWaiter::RobotWaiter(const int &x, const int &y, const int &id, World* world)
  :Object(x, y, id, world), birthPlace(x,y), rules(), world(world)
{
    load_image("images/robotwaiter.bmp");
    cafe = static_cast<Cafe*>(world);
    commandControlled = false;
    action = GetOrder;
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
    switch(action)
    {
    case Waiting:
        if(!commandControlled)
        {
            customers = cafe->customers;
            GridSearch searcher(world->getWorldGrid(),&rules);
            Customer* customer = getClosestCustomer();
            if(customer!=NULL)
            {
                GridSearch::Node customerNode(customer->getx(),customer->gety());
                path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                action = GetOrder;
                return;
            }
        } else {
            if(customers.begin()!=customers.end())
            {
                GridSearch searcher(world->getWorldGrid(),&rules);
                GridSearch::Node customerNode(customers.front()->getx(),customers.front()->gety());
                path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                action = GetOrder;
                return;
            }
        }
        if(path.empty() && getx()!=birthPlace.first && gety()!=birthPlace.second)
        {
            GridSearch searcher(world->getWorldGrid(), &defaultRules);
            path = searcher.BFS(GridSearch::Node(getx(),gety()), birthPlace, GridSearch::NullID);
        } else if(!path.empty()){
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldMap();
            if(move(node.first,node.second))
            {
                path.erase(path.begin());
            }
            else
            {
                //We want the waiter to wait for the server to go by. This will hopefully ensure that the robots
                //don't get into a endless loop of up/down which I have seen
                //if(worldMap[node.first][node.second]!=WorldObjects::ROBOTSERVER)
                //    path.clear();
                if(worldMap[node.first][node.second]==WorldObjects::ROBOTSERVER)
                {
                    //Do nothing since we are waiting for the robot server go around us
                } else if(worldMap[node.first][node.second]==WorldObjects::ROBOTWAITER) {
                    // Strict hierchy of whoever is first in the list can keep moving while the other replans
                    for(std::vector<RobotWaiter*>::iterator it=cafe->robotwaiters.begin(); it!=cafe->robotwaiters.end(); it++)
                    {
                        if((*it)==this)
                            break;
                        if((*it)->getx()==node.first && (*it)->gety()==node.second)
                            path.clear();
                    }
                } else {
                    path.clear();
                }
            }
        }

        break;
    case GetOrder:
        if(path.empty())
        {
            if(!commandControlled)
            {
                customers = cafe->customers;
                GridSearch searcher(world->getWorldGrid(),&rules);
                Customer* customer = getClosestCustomer();
                if(customer!=NULL)
                {
                    GridSearch::Node customerNode(customer->getx(),customer->gety());
                    path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                    if(path.empty())
                    {
                        action = Waiting;
                    }
                } else {
                    action = Waiting;
                }
            } else {
                if(customers.begin()!=customers.end())
                {
                    GridSearch searcher(world->getWorldGrid(),&rules);
                    GridSearch::Node customerNode(customers.front()->getx(),customers.front()->gety());
                    path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                    if(path.empty())
                    {
                        action = Waiting;
                    }
                } else {
                    action = Waiting;
                }
            }
        } else {
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldMap();
            if(path.size()<=2 && worldMap[node.first][node.second]==WorldObjects::CUSTOMER)
            {
                Customer* customer = findCustomer(node.first, node.second);
                if(customer!=NULL)
                {
                    int order = customer->askForOrder();
                    if(order!=-1 && order!=-2)
                    {
                        cafe->orders.push_back(order);
                        path.clear();
                        cafe->visited.push_back(customer->getIdentifer());
                        cafe->orderMap.push_back(std::pair<Customer*, int>(customer, order));
                    }
                    //Order already taken
                    if(order==-2)
                    {
                        path.clear();
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
                //We want the waiter to wait for the server to go by. This will hopefully ensure that the robots
                //don't get into a endless loop of up/down which I have seen
                if(worldMap[node.first][node.second]==WorldObjects::ROBOTSERVER)
                {
                    //Do nothing since we are waiting for the robot server go around us
                } else if(worldMap[node.first][node.second]==WorldObjects::ROBOTWAITER) {
                    // Strict hierchy of whoever is first in the list can keep moving while the other replans
                    for(std::vector<RobotWaiter*>::iterator it=cafe->robotwaiters.begin(); it!=cafe->robotwaiters.end(); it++)
                    {
                        if((*it)==this)
                            break;
                        if((*it)->getx()==node.first && (*it)->gety()==node.second)
                            path.clear();
                    }
                } else {
                    path.clear();
                }
            }
        }
        break;
    }
}

Customer* RobotWaiter::getClosestCustomer()
{
    Customer* nully = NULL;
    int distance(Screen::SCREEN_HEIGHT), temp(0);
    int x = getx(), y = gety(), x2(0), y2(0);
    for(std::vector<Customer*>::iterator it = customers.begin(); it!=customers.end(); it++)
    {
        if(std::find(cafe->visited.begin(),cafe->visited.end(),(*it)->getIdentifer())==cafe->visited.end())
        {
            x2 = (*it)->getx();
            y2 = (*it)->gety();
            temp = sqrt(pow(x2-x,2)+pow(y2-y,2));
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
    for(std::vector<Customer*>::iterator it = customers.begin(); it!=customers.end(); it++)
    {
        if((*it)->getx()==x && (*it)->gety()==y)
            return (*it);
    }
    return nully;
}

void RobotWaiter::setAction(Action act)
{
    action = act;
}

void RobotWaiter::addCustomer(Customer* _customer)
{
    customers.push_back(_customer);
}
