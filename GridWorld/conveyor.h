#ifndef CONVEYOR_H
#define CONVEYOR_H

#include "object.h"


class Conveyor : public Object
{
    public:
        Conveyor(const int &x, const int &y, const int &id, World* world);
        virtual ~Conveyor();
        void execute();
    protected:
    private:
        int imageNum;
        SDL_Surface* image1;
        SDL_Surface* image2;
};

#endif // CONVEYOR_H
