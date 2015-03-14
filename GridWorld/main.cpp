#include <SDL.h>
#include "World.h"

int main( int argc, char* args[] )
{
    World world("map.dd");
    if(world.successful()==false)
        return 1;
    world.start();
	return 0;
}
