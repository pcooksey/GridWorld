#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <cstdlib>
#include <assert.h>
#include "world.h"

namespace ObjectBody {
    /// The attributes of the object
    enum Type {ALIVE, INANIMATE};
}

class World;

class Object
{
    public:
        Object(const int &x, const int &y, const int &id, World* world);
        virtual ~Object();

        int getx() const { return x; }
        int gety() const { return y; }
        int getId() const { return id; }

        /** Sets the @color for the object in the world */
        void setColor(int red, int blue, int green, int alpha);
        /** Sets the @bodyImage for the object in the world */
        void load_image( std::string filename );
        /** Set the @bodyImage with a texture (used for animation) */
        void set_image(SDL_Surface* image);
    protected:
        /** Attempt to move the object in the world */
        bool move(int x, int y);

        /** The @Object can execute actions during the world frame here */
        virtual void execute() = 0;

        /** Body type is by default real */
        ObjectBody::Type type;
    private:
        /** Position of the object that can't be change manually
            only the world can change the values */
        int x, y;
        int id;
        /** The color of the block or @Object */
        SDL_Color color;
        /** The image used for the body */
        SDL_Texture* bodyImage;
        /** Pointer to the world that contains the @Object */
        World* world;

        friend World;
};

#endif // OBJECTS_H
