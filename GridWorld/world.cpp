#include "world.h"

World::World(std::string fileName)
 : success(false)
{
    window = NULL;
    screen = NULL;
    renderer = NULL;
    success = init();
    gridMap.open(fileName);
}

World::~World()
{
    //Destroy window
    SDL_DestroyRenderer( renderer );
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

    //Create renderer for window
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

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

        //Start drawing the grid world from the map
        Map::MapSize mapSize = gridMap.getSize();
        Map::MapSize gridSize(int(SCREEN_HEIGHT/mapSize.first),int(SCREEN_WIDTH/mapSize.second));

        // Drawing the map here
        int value;
        const Map::MultiArray grid = gridMap.getGrid();
        SDL_Rect fillRect;
        for(int x=0; x<mapSize.first; x++)
            for(int y=0; y<mapSize.second; y++)
            {
                value = grid[x][y];
                switch(value) {
                    case 4:
                        SDL_SetRenderDrawColor( renderer, 0xff, 0x00, 0x00, 0x00 );
                        break;
                    case 3:
                        SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0x00 );
                        break;
                    case 2:
                        SDL_SetRenderDrawColor( renderer, 0x00, 0xff, 0xff, 0xff );
                        break;
                    case 1:
                        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
                        break;
                    case 0:
                    default:
                        SDL_SetRenderDrawColor( renderer, 0xd3, 0xd3, 0xd3, 0xd3 );
                }
                fillRect = { gridSize.second*y, gridSize.first*x, gridSize.second, gridSize.first };
                SDL_RenderFillRect( renderer, &fillRect );

            }

        for(std::vector<Object*>::iterator it = objects.begin(); it!=objects.end(); it++)
        {
            (*it)->execute();
            SDL_Color color = (*it)->color;
            SDL_SetRenderDrawColor( renderer, color.r, color.b, color.g, color.a);
            fillRect = { gridSize.second*(*it)->y, gridSize.first*(*it)->x, gridSize.second, gridSize.first };
            SDL_RenderFillRect( renderer, &fillRect );
        }

        //Draw black grid lines
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
        int start(0);
        while(start<SCREEN_HEIGHT)
        {
            SDL_RenderDrawLine( renderer, 0, start, SCREEN_WIDTH, start );
            start += gridSize.first;
        }
        start = 0;
        while(start<SCREEN_WIDTH)
        {
            SDL_RenderDrawLine( renderer, start, 0, start, SCREEN_HEIGHT);
            start += gridSize.second;
        }

        //Update screen
        SDL_RenderPresent( renderer );
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

bool const World::move(Object* object, int x, int y)
{
    Map::MapSize mapSize = gridMap.getSize();
    const Map::MultiArray grid = gridMap.getGrid();
    if(x>mapSize.first || y>mapSize.second)
        return false;
    if(grid[x][y]==1)
        return false;
    for(std::vector<Object*>::iterator it=objects.begin(); it!= objects.end(); it++)
    {
        // Does not check if the same object because object move should stop this from happening
        if((*it)->x==x && (*it)->y==y)
            return false;
    }
    object->x = x;
    object->y = y;
    return true;
}

void World::addObject(Object* temp)
{
    objects.push_back(temp);
}
