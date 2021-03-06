#include "customer.h"

bool CustomerRules::check(GridSearch* searcher, int id)
{
    if(getGoal(searcher)==GridSearch::NullNode) {
        if(id==OPENSPACE || id==DOORWAY || (getId(searcher)!=DOORWAY && id==TABLE))
            return true;
    } else {
        if(id==OPENSPACE || id==DOORWAY)
            return true;
        if(id==TABLE)
        {
            if(getGoal(searcher)==getCurrent(searcher))
                return true;
        }
    }
    return false;
}


Customer::Customer(const int &x, const int &y, const int &id, int identifer, World* world)
:Object(x, y, id, world), identifer(identifer), orderRandomTime((rand()%10)+1), rules(), world(world)
{
    load_image("images/human1.bmp");
    readyToLeave = false;
    //Noticed that I am using the grid without any objects
    //Behavior changes with world->getWorldGrid() however I should
    //change it to this eventually
    GridSearch searcher(world->getGrid(),&rules);
    //path = searcher.DFS(GridSearch::Node(x,y), GridSearch::Node(9,4));
    path = searcher.DFS(GridSearch::Node(x,y), GridSearch::NullNode, TABLE);
}

Customer::~Customer()
{
    //dtor
}

void Customer::execute()
{
    if(readyToLeave)
    {
        if(path.size()!=0)
        {
            GridSearch::Node node = path.front();
            if(move(node.first,node.second))
            {
                path.erase(path.begin());
            } else {
                path.clear();
            }
        } else {
            GridSearch searcher(world->getWorldGrid(),&rules);
            path = searcher.BFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, DOORWAY);
        }
    }
    if(!path.empty())
    {
        GridSearch::Node node = path.front();
        if(move(node.first,node.second))
        {
            path.erase(path.begin());
        } else {
            //If path size == 1 then we are at the table. Otherwise
            //something got in the customers way so re-plan path
            if(path.size()!=1)
            {
                GridSearch searcher(world->getWorldGrid(),&rules);
                path = searcher.DFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, TABLE);
                node = path.front();
                move(node.first,node.second);
            }
        }
    } else {
        if(readyToLeave)
        {
            GridSearch searcher(world->getWorldGrid(),&rules);
            path = searcher.BFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, DOORWAY);
        } else {
            GridSearch searcher(world->getWorldGrid(),&rules);
            path = searcher.DFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, TABLE);
        }
    }
}

int Customer::askForOrder()
{
    //Should only have to ask once
    if(orderRandomTime==0)
    {
        orderRandomTime--;
        return WorldObjects::HAMBURGER;
    } else if(orderRandomTime<0) {
        return -2;
    } else {
        orderRandomTime--;
        return -1;
    }
}

bool Customer::receiveFood(int food)
{
    if(food==HAMBURGER)
    {
        GridSearch searcher(world->getWorldGrid(),&rules);
        path = searcher.BFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, DOORWAY);
        readyToLeave = true;
        return true;
    }
    return false;
}
