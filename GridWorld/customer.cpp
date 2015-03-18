#include "customer.h"

Customer::Customer(const int &x, const int &y, const int &id, World* world)
:Object(x, y, id, world), rules(), world(world)
{
    GridSearch searcher(world->getGrid(),&rules);
    //path = searcher.BFS(GridSearch::Node(x,y), GridSearch::Node(x+7,y+10));
    path = searcher.BFS(GridSearch::Node(x,y), GridSearch::NullNode, 3);
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
        }
    }
}
