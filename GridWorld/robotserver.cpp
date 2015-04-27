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
  :Object(x, y, id, world), birthPlace(x,y), rules(), world(world)
{
    load_image("images/robotserver.bmp");
    cafe = static_cast<Cafe*>(world);
    action = GetFood;
    commandControlled = false;
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
                path.clear();
            }
        } else {
            if(!commandControlled)
            {
                GridSearch searcher(world->getWorldGrid(),&rules);
                GridSearch::Node pickUpNode(cafe->robotArm->getx(),cafe->robotArm->gety());
                path = searcher.BFS(GridSearch::Node(getx(),gety()), pickUpNode, GridSearch::NullID);
                if(!path.empty())
                {
                    action = GetFood;
                }
            } else {
                if(customers.size()>0)
                {
                    GridSearch searcher(world->getWorldGrid(),&rules);
                    GridSearch::Node pickUpNode(cafe->robotArm->getx(),cafe->robotArm->gety());
                    path = searcher.BFS(GridSearch::Node(getx(),gety()), pickUpNode, GridSearch::NullID);
                    if(!path.empty())
                    {
                        action = GetFood;
                    }
                }
            }
        }
        break;
    case GetFood:
        if(path.empty())
        {
            GridSearch searcher(world->getWorldGrid(),&rules);
            GridSearch::Node pickUpNode(cafe->robotArm->getx(),cafe->robotArm->gety());
            path = searcher.BFS(GridSearch::Node(getx(),gety()), pickUpNode, GridSearch::NullID);
            if(path.empty())
            {
                action = Nothing;
            }
        } else {
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldGrid().getGrid();
            if(path.size()<2 && worldMap[node.first][node.second]==WorldObjects::PICKUP)
            {
                if(!commandControlled)
                {
                    if(food.size()<3)
                    {
                        int foodNum = cafe->robotArm->grabFood();
                        if(foodNum!=0)
                        {
                            food.push_back(foodNum);
                        } else if(food.size()>0) {
                            action = ServerFood;
                        }
                    } else {
                        action = ServerFood;
                        path.clear();
                    }
                } else {
                    if(customers.size()!=0)
                    {
                        if(food.size()<3)
                        {
                            int foodNum = cafe->robotArm->grabFood();
                            if(foodNum!=0)
                            {
                                food.push_back(foodNum);
                            } else if(food.size()>0 && customers.size()<=food.size()) {
                                action = ServerFood;
                            }
                        } else {
                            action = ServerFood;
                            path.clear();
                        }
                    } else {
                        action = Nothing;
                    }
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
            if(!commandControlled)
            {
                if(cafe->orderMap.begin()!=cafe->orderMap.end())
                {
                    GridSearch searcher(world->getWorldGrid(),&rules);
                    GridSearch::Node customerNode(cafe->orderMap.front().first->getx(),cafe->orderMap.front().first->gety());
                    path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                }
            } else {
                if(customers.begin()!=customers.end())
                {
                    GridSearch searcher(world->getWorldGrid(),&rules);
                    GridSearch::Node customerNode(customers.front()->getx(),customers.front()->gety());
                    path = searcher.BFS(GridSearch::Node(getx(),gety()), customerNode, GridSearch::NullID);
                } else {
                    action = Nothing;
                }
            }
        } else {
            GridSearch::Node node = path.front();
            const Map::MultiArray& worldMap = world->getWorldGrid().getGrid();
            if(path.size()<2 && worldMap[node.first][node.second]==WorldObjects::CUSTOMER)
            {
                if(!commandControlled)
                {
                    bool success = cafe->orderMap.begin()->first->receiveFood(food[0]);
                    if(success)
                    {
                        food.erase(food.begin());
                        cafe->orderMap.erase(cafe->orderMap.begin());
                        path.clear();
                    }
                    if(food.size()==0)
                    {
                        action = GetFood;
                        path.clear();
                    }
                } else {
                    bool success = customers.front()->receiveFood(food[0]);
                    if(success)
                    {
                        std::pair<Customer*, int> customer(customers.front(), food.front());
                        food.erase(food.begin());
                        cafe->orderMap.erase(std::find(cafe->orderMap.begin(),cafe->orderMap.end(),customer));
                        customers.erase(customers.begin());
                        path.clear();
                    }
                    if(food.size()==0)
                    {
                        action = GetFood;
                        path.clear();
                    }
                }
            }
            else if(move(node.first,node.second)) {
                path.erase(path.begin());
            } else {
                //path.clear();
                if(worldMap[node.first][node.second]==WorldObjects::ROBOTSERVER)
                {
                    // Strict hierchy of whoever is first in the list can keep moving while the other replans
                    for(std::vector<RobotServer*>::iterator it=cafe->robotservers.begin(); it!=cafe->robotservers.end(); it++)
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

void RobotServer::setAction(Action act)
{
    action = act;
}

void RobotServer::addCustomer(Customer* _customer)
{
    customers.push_back(_customer);
}
