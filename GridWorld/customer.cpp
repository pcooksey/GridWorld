#include "customer.h"

bool CustomerRules::check(GridSearch* searcher, int id)
{
    if(getGoal(searcher)==GridSearch::NullNode) {
        if(id==OPENSPACE || id==DOORWAY || id==TABLE)
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


Customer::Customer(const int &x, const int &y, const int &id, World* world)
:Object(x, y, id, world), rules(), world(world)
{
    load_image("images/human1.bmp");
    //Noticed that I am using the grid without any objects
    //Behavior changes with world->getWorldGrid() however I should
    //change it to this eventually
    GridSearch searcher(world->getGrid(),&rules);
    //path = searcher.DFS(GridSearch::Node(x,y), GridSearch::Node(x-6,y-4));
    path = searcher.DFS(GridSearch::Node(x,y), GridSearch::NullNode, TABLE);
}

Customer::~Customer()
{
    //dtor
}

void Customer::execute()
{
    //move(getx()+((rand()%2-1?-1:1)*(rand()%2-1)),gety()+((rand()%2-1?-1:1)*(rand()%2-1)));
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
                GridSearch searcher(world->getGrid(),&rules);
                path = searcher.DFS(GridSearch::Node(getx(),gety()), GridSearch::NullNode, TABLE);
                node = path.front();
                move(node.first,node.second);
            }
        }
    }
}
