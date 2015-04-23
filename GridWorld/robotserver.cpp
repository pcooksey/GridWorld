#include "robotserver.h"

bool RobotServerRules::check(GridSearch* searcher, int id)
{
    if(getGoal(searcher)==GridSearch::NullNode) {
        assert(false);
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
        if((id==CUSTOMER || id==PICKUP) && getGoal(searcher)==getChecking(searcher))
        {
            return true;
        }
    }
    return false;
}

RobotServer::RobotServer(const int &x, const int &y, const int &id, World* world)
  :Object(x, y, id, world), rules(), world(world)
{
    load_image("images/robotserver.bmp");
    cafe = static_cast<Cafe*>(world);
    action = GetFood;
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
        if(path.empty())
        {
            GridSearch searcher(world->getWorldGrid(),&rules);
            GridSearch::Node pickUpNode(cafe->robotArm->getx(),cafe->robotArm->gety());
            path = searcher.BFS(GridSearch::Node(getx(),gety()), pickUpNode, GridSearch::NullID);
        } else {
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldGrid().getGrid();
            if(path.size()<=2 && worldMap[node.first][node.second]==WorldObjects::PICKUP)
            {
                if(food.size()<3)
                {
                    int foodNum = cafe->robotArm->grabFood();
                    if(foodNum!=0)
                    {
                        food.push_back(foodNum);
                    }
                } else {
                    action = ServerFood;
                    path.clear();
                }
            }
            else if(move(node.first,node.second)) {
                path.erase(path.begin());
            } else {
                path.clear();
            }
        }
        break;
    case ServerFood:
        if(path.empty())
        {
            if(cafe->orderMap.begin()!=cafe->orderMap.end())
            {
                GridSearch searcher(world->getWorldGrid(),&rules);
                GridSearch::Node customerNode(cafe->orderMap.front().first->getx(),cafe->orderMap.front().first->gety());
                path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
            }
        } else {
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldGrid().getGrid();
            if(path.size()<=2 && worldMap[node.first][node.second]==WorldObjects::CUSTOMER)
            {
                if(food.size()>0)
                {
                    bool success = cafe->orderMap.begin()->first->receiveFood(food[0]);
                    if(success)
                    {
                        food.erase(food.begin());
                        cafe->orderMap.erase(cafe->orderMap.begin());
                        path.clear();
                    }
                } else {
                    action = GetFood;
                    path.clear();
                }
            }
            else if(move(node.first,node.second)) {
                path.erase(path.begin());
            } else {
                path.clear();
            }
        }
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
