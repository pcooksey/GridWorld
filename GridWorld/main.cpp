#include <SDL.h>
#include <stdlib.h>
#include <ctime>
#include "cafe.h"
#include "customer.h"
#include <iostream>

int main( int argc, char* args[] )
{
    /* initialize random seed: */
    srand ((unsigned)time(NULL));
    Cafe cafe("map.dd");
    if(cafe.successful()==false)
        return 1;
    cafe.start();
	return 0;
}
