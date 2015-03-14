#include "world.h"

World::World(int screen_width, int screen_height, int screen_bpp)
{
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    SCREEN_BPP = screen_bpp;
    screen = NULL;
    success = init();
}

World::~World()
{
    SDL_DestroySemaphore( runLock ); //Destroy semaphore
    SDL_Quit(); //Quit SDL
}

bool World::init()
{
    // Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

    // Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    // If there was an error in setting up the screen
    if( screen == NULL )
        return false;

    // Create the semaphore
    runLock = SDL_CreateSemaphore( 1 );

    // Set the window caption
    SDL_WM_SetCaption( "Grid World", NULL );

    // If everything initialized fine
    return true;
}

void World::start()
{
    bool quit = false;
    running = true;
    SDL_Thread *thread = SDL_CreateThread( starter, this );
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
    return ((World*)data)->screenthread();
}

int World::screenthread()
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

        // Update Screen
        if(SDL_Flip( screen ) == -1)
            return 1;
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
