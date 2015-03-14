#include "world.h"

int main ( int argc, char** argv )
{
    World world(640,480,32);
    if(world.successful()==false)
        return 1;
    world.start();
    return 0;
}
