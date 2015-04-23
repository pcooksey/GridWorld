#include "cafe.h"

Cafe::Cafe(std::string fileName)
:World(fileName), gridMap(getGrid()), belt(CONVEYORBELT,this)
{
    setWorldTimeSpeed(50);
}

Cafe::~Cafe()
{
    delete kitchen;
    delete chef;
    delete robotArm;
    for(std::vector<Customer*>::iterator it=customers.begin(); it!=customers.end(); it++)
        delete (*it);
    for(std::vector<RobotWaiter*>::iterator it=robotwaiters.begin(); it!=robotwaiters.end(); it++)
        delete (*it);
}

bool Cafe::successful()
{
    return World::successful();
}

void Cafe::start()
{
    //Find objects TODO: going to need to find table seats to make robot search quicker
    findDoors();
    // Create inanimate objects
    belt.start();
    kitchen = new Kitchen(KITCHENCOUNTER,this);
    kitchen->start();
    //Create real moving things
    chef = new Chef(4,14,CHEF,this);
    addObject(chef);
    createCustomer();
    createCustomer();
    createRobotWaiter(9,9);
    createRobotServer(9,10);
    createRobotArm();

    updateWorldMap();
    return World::start();
}

void Cafe::execute()
{
    if(rand()%20==0)
    {
        createCustomer();
    }
    belt.execute();
    if(!orders.empty())
    {
        if(kitchen->addFood(orders.front()))
        {
            orders.erase(orders.begin());
        }
    }
    kitchen->execute();
    for(std::vector<Customer*>::iterator it=customers.begin(); it!=customers.end(); it++)
    {
        Node temp((*it)->getx(), (*it)->gety());
        if((*it)->readyToLeave && std::find(doorways.begin(), doorways.end(), temp)!=doorways.end())
        {
            delete (*it);
            removeObject((*it));
            customers.erase(it);
            it--;
        }
    }
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

void Cafe::createRobotWaiter(int x, int y)
{
    RobotWaiter* robot = new RobotWaiter(x,y,WorldObjects::ROBOTWAITER,this);
    robotwaiters.push_back(robot);
    addObject(robot);
}

void Cafe::createRobotServer(int x, int y)
{
    RobotServer* robot = new RobotServer(x,y,WorldObjects::ROBOTSERVER,this);
    robotservers.push_back(robot);
    addObject(robot);
}

void Cafe::createRobotArm()
{
    robotArm = new RobotArm(6,14,WorldObjects::PICKUP,this);
    addObject(robotArm);
}
