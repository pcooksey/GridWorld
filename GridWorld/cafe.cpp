#include "cafe.h"

Cafe::Cafe(std::string fileName)
:World(fileName), gridMap(getGrid())
{
    //ctor
}

Cafe::~Cafe()
{
    //dtor
}

bool Cafe::successful()
{
    return World::successful();
}

void Cafe::start()
{
    findDoors();
    createCustomer();
    createCustomer();
    return World::start();
}

void Cafe::execute()
{
    if(rand()%20==0)
    {
        createCustomer();
    }
    /*
    if(!customers.empty())
    {
        removeObject(customers.front());
        delete (*customers.begin());
        customers.erase(customers.begin());
    }
    */
}

void Cafe::findDoors()
{
    Map::MapSize mapSize = gridMap.getSize();
    int value;
    const Map::MultiArray& grid = gridMap.getGrid();
    for(int x=0; x<mapSize.first; x++)
        for(int y=0; y<mapSize.second; y++)
        {
            value = grid[x][y];
            switch(value) {
                case 3: // Door ways
                    doorways.push_back(Node(x,y));
                    break;
                default:
                    break;
            }
        }
}

Cafe::Node Cafe::getADoor()
{
    std::random_shuffle ( doorways.begin(), doorways.end() );
    return doorways.front();
}

void Cafe::createCustomer()
{
    Node node = getADoor();
    Customer* customer = new Customer(node.first,node.second,6,this);
    customers.push_back(customer);
    addObject(customer);
}
