#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>
#include <SDL_thread.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "map.h"
#include "object.h"

namespace Screen {
    /// The attributes of the screen
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;
}

using namespace Screen;

class Object;

class World
{
    public:
        World(std::string fileName);
        virtual ~World();

        bool successful() const { return success; };
        /** Controls the event loop of the world */
        void start();
        /** Add @Object to the world */
        void addObject(Object* temp);
        /** Remove @Object from the world */
        void removeObject(Object* temp);
        /** Get the @gridMap for searching */
        const Map& getGrid() {return gridMap;};
        /** Get the @worldMap for searching */
        const Map::MultiArray& getWorldMap() {return worldMap;};

    protected:
        bool const move (Object* object, int x, int y);

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
        /** Updates the @worldMap */
        void updateWorldMap();

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
        /** Map of everything in world (includes objects) */
        Map::MultiArray worldMap;

        std::vector<Object*> objects;

        friend Object;
};

#endif // WORLD_H
