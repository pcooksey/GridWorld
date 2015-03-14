#ifndef WORLD_H
#define WORLD_H

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

class World
{
    public:
        World(int screen_width, int screen_height, int screen_bpp);
        virtual ~World();

        bool successful() const { return success; };
        /** Controls the event loop of the world */
        void start();

    protected:

    private:
        /**  Starts SDL and initializes all components */
        bool init();
        /** Controls the screen in another thread */
        int screenthread();
        /** Starts the thread that controls the screen */
        static int starter(void *data);
        /** thread protect function for setting running variable */
        void setRunning(bool run);
        /** thread protect function for getting running variable */
        bool getRunning();


        /** Is true when the init was successful */
        bool success;
        /** When false it signals for the @start thread to end */
        bool running;
        /** The protective semaphore for @running */
        SDL_sem *runLock;
        /** SDL screen to update the screen displayed */
        SDL_Surface* screen;

        /** The attributes of the screen */
        int SCREEN_WIDTH;// = 640;
        int SCREEN_HEIGHT;// = 480;
        int SCREEN_BPP;// = 32;

};

#endif // WORLD_H
