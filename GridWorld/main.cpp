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
    Customer first(10,5,&world);
    first.load_image("human1.bmp");
    world.addObject(&first);
    if(world.successful()==false)
        return 1;
    world.start();
	return 0;
}
