#include "cafe.h"

Cafe::Cafe(std::string fileName)
:World(fileName), gridMap(getGrid()), belt(CONVEYORBELT,this)
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
    createRobotWaiter();
    belt.start();
    //belt.addFood(WorldObjects::HAMBURGER);
    return World::start();
}

void Cafe::execute()
{
    if(rand()%20==0)
    {
        //belt.addFood(WorldObjects::HAMBURGER);
        createCustomer();
    }
    belt.execute();
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
                case WorldObjects::DOORWAY: // Door ways
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
    Customer* customer = new Customer(node.first,node.second,WorldObjects::CUSTOMER,this);
    customers.push_back(customer);
    addObject(customer);
}

void Cafe::createRobotWaiter()
{
    RobotWaiter* robot = new RobotWaiter(9,9,WorldObjects::ROBOTWAITER,this);
    robotwaiters.push_back(robot);
    addObject(robot);
}
