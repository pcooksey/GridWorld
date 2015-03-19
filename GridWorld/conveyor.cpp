#include "conveyor.h"

Conveyor::Conveyor(const int &x, const int &y, const int &id, World* world)
 :Object(x, y, id, world)
{
    type = ObjectBody::INANIMATE;
    image1 = SDL_LoadBMP( "images/conveyor1.bmp");
    image2 = SDL_LoadBMP( "images/conveyor2.bmp" );
    imageNum = 1;
    set_image(image1);
}

Conveyor::~Conveyor()
{
    SDL_FreeSurface( image1 );
    SDL_FreeSurface( image2 );
}

void Conveyor::execute()
{
    if(imageNum<10) {
        set_image(image1);
        imageNum++;
    } else if(imageNum<20) {
        set_image(image2);
        imageNum++;
    } else {
        imageNum = 0;
    }
}
