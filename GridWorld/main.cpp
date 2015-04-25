#include <SDL.h>
#include "cafe.h"
#include "customer.h"
#include <stdlib.h>
#include <time.h>

int main( int argc, char* args[] )
{
    /* initialize random seed: */
    srand (time(0));
    Cafe cafe("map.dd");
    if(cafe.successful()==false)
        return 1;
    cafe.start();
	return 0;
}
