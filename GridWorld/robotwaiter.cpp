#include "robotwaiter.h"

bool RobotWaiterRules::check(GridSearch* searcher, int id)
{
    if(getGoal(searcher)==GridSearch::NullNode) {
        if(id==OPENSPACE || id==DOORWAY || id==CUSTOMER)
            return true;
    } else {
        if(id==OPENSPACE || id==DOORWAY)
            return true;
        if(id==CUSTOMER)
        {
            if(getGoal(searcher)==getCurrent(searcher))
                return true;
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

*/

void RobotWaiter::execute()
{
    if(path.empty())
    {
        customers = cafe->customers;
        GridSearch searcher(world->getWorldGrid(),&rules);
        path = searcher.BFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, WorldObjects::CUSTOMER);
    } else {
        GridSearch::Node node = path.front();
        if(move(node.first,node.second))
        {
            path.erase(path.begin());
        }
    }
}