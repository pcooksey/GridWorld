#include "world.h"

World::World()
 : success(false)
{
    window = NULL;
    screen = NULL;
    success = init();
}

World::~World()
{
    //Destroy window
	SDL_DestroyWindow( window );
    SDL_DestroySemaphore( runLock ); //Destroy semaphore
    SDL_Quit(); //Quit SDL
}

bool World::init()
{
    // Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        return false;

    //Create window
    window = SDL_CreateWindow( "Grid World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    // If there was an error in setting up the screen
    if( window == NULL )
        return false;
    // Set up the screen
    screen = SDL_GetWindowSurface( window );

    // Create the semaphore
    runLock = SDL_CreateSemaphore( 1 );

    // If everything initialized fine
    return true;
}

void World::start()
{
    bool quit = false;
    running = true;
    SDL_Thread *thread = SDL_CreateThread( starter, "ScreenThread", this );
    SDL_Event event;
    while(!quit)
    {
        // Check if user wants to quit
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
                setRunning(false); // Stop the thread
                SDL_WaitThread( thread, NULL ); //Waits for thread to end
            }
        }
    }
}

int World::starter(void *data)
{
    return ((World*)data)->screenThread();
}

int World::screenThread()
{
    // The frame rate regulator
    int delta = SDL_GetTicks(), time;

    while(getRunning())
    {
        // Time (dt) between frames **No need for fps**
        time = (SDL_GetTicks() - delta);
        // Start clock over for time (dt)
        delta = SDL_GetTicks();

        // Clean the screen to black
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

        //Update the surface
        SDL_UpdateWindowSurface( window );
    }

    return 0;
}

void World::setRunning(bool run)
{
    SDL_SemWait( runLock ); //Lock
    running = run;
    SDL_SemPost( runLock ); //unlock
}

bool World::getRunning()
{
    SDL_SemWait( runLock ); //Lock
    bool run = running;
    SDL_SemPost( runLock ); //unlock
    return run;
}
