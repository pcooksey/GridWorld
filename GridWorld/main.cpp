#include <SDL.h>
#include "world.h"
#include "customer.h"
#include <stdlib.h>
#include <time.h>

int main( int argc, char* args[] )
{
    /* initialize random seed: */
    srand (time(NULL));
    World world("map.dd");
    Customer first(19,5,6,&world);
    Customer second(19,4,6,&world);
    world.addObject(&first);
    world.addObject(&second);
    if(world.successful()==false)
        return 1;
    world.start();
	return 0;
}
