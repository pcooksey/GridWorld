#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>
#include <SDL_thread.h>
#include <string>

#include "map.h"

namespace Screen {
    /// The attributes of the screen
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;
}

using namespace Screen;

class World
{
    public:
        World(std::string fileName);
        virtual ~World();

        bool successful() const { return success; };
        /** Controls the event loop of the world */
        void start();

    protected:
        //virtual

    private:
        /** Creates all SDL components */
        bool init();
        /** Controls the screen in another thread */
        int screenThread();
        /** Starts the thread that controls the screen */
        static int starter(void *data);
        /** Thread protected to set @running */
        void setRunning(bool run);
        /** Thread protected to get @running */
        bool getRunning();
        /** Is true when the init was successful */
        bool success;
        /**  When false it signals for the @start thread to end */
        bool running;
        /** The protective semaphore for @running */
        SDL_sem *runLock;

        /** SDL window for world */
        SDL_Window* window;
        /** SDL Screen for manipulation */
        SDL_Surface* screen;
        /** The window renderer */
        SDL_Renderer* renderer;

        /** Map of the world */
        Map gridMap;
};

#endif // WORLD_H
