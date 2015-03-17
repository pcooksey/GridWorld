#include "object.h"

Object::Object(const int &x, const int &y, World* world)
    :x(x),y(y),world(world)
{
    type = ObjectBody::ALIVE;
    bodyImage = NULL;
    setColor(0,0,0,255);
}

Object::~Object()
{
    SDL_DestroyTexture( bodyImage );
}

void Object::setColor(int red, int blue, int green, int alpha)
{
    color.r = red;
    color.b = blue;
    color.g = green;
    color.a = alpha;
}

bool Object::move(int x, int y)
{
    //First make sure that the object isn't trying to move more than one square
    if(type==ObjectBody::INANIMATE ||
       abs(this->x-x)>1 || abs(this->y-y)>1 ||
       (abs(this->x-x)==1 && abs(this->y-y)==1))
        return false;
    if(this->x==x && this->y==y)
        return true;
    return world->move((this), x, y);
}

 void Object::load_image( std::string filename )
{
    SDL_Surface* loadedImage = SDL_LoadBMP( filename.c_str() );
    if( loadedImage != NULL )
    {
        //Color key image
		SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGB( loadedImage->format, 0, 0xFF, 0xFF ) );
        bodyImage = SDL_CreateTextureFromSurface( world->renderer, loadedImage );
        assert(bodyImage!=NULL);
        SDL_FreeSurface( loadedImage );
    }
}
