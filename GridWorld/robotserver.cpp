#include "robotserver.h"

bool RobotServerRules::check(GridSearch* searcher, int id)
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

RobotServer::RobotServer(const int &x, const int &y, const int &id, World* world)
  :Object(x, y, id, world), world(world)
{
    load_image("images/robotserver.bmp");
    cafe = static_cast<Cafe*>(world);
    action = Nothing;
}

RobotServer::~RobotServer()
{
    //dtor
}

void RobotServer::execute()
{
    switch(action)
    {
    case Nothing:
        break;
    case GetFood:
        break;
    case ServerFood:
        break;
    }
}

void RobotServer::setAction(Action act)
{
    action = act;
}

void RobotServer::addCustomer(Customer* _customer)
{
    customers.push_back(_customer);
}
