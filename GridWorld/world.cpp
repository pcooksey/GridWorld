#include "world.h"

World::World(std::string fileName)
 : success(false)
{
    window = NULL;
    screen = NULL;
    renderer = NULL;
    success = init();
    gridMap.open(fileName);
    timeSpeed = 50;
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
        // Start clock over for time (dt)
        delta = SDL_GetTicks();

        // Clean the screen to black
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

        //Start drawing the grid world from the map
        Map::MapSize mapSize = gridMap.getSize();
        Map::MapSize gridSize(int(SCREEN_HEIGHT/mapSize.first),int(SCREEN_WIDTH/mapSize.second));

        // Drawing the map here
        int value;
        const Map::MultiArray& grid = gridMap.getGrid();
        SDL_Rect fillRect;
        for(int x=0; x<mapSize.first; x++)
            for(int y=0; y<mapSize.second; y++)
            {
                value = grid[x][y];
                switch(value) {
                    case PICKUP: // Pick-up food zone
                        SDL_SetRenderDrawColor( renderer, 0xff, 0x00, 0x00, 0x00 );
                        break;
                    case DOORWAY: // Door ways
                        SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0x00 );
                        break;
                    case TABLE: // Tables
                        //5C4033
                        SDL_SetRenderDrawColor( renderer, 0x66, 0x33, 0x00, 0xff );
                        break;
                    case WALL: // Walls
                        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
                        break;
                    case OPENSPACE: // Open space
                    default:
                        SDL_SetRenderDrawColor( renderer, 0xd3, 0xd3, 0xd3, 0xd3 );
                }
                fillRect = { gridSize.second*y, gridSize.first*x, gridSize.second, gridSize.first };
                SDL_RenderFillRect( renderer, &fillRect );

            }

        // Color all objects into the world
        for(std::vector<Object*>::iterator it = objects.begin(); it!=objects.end(); it++)
        {
            (*it)->execute();
            if((*it)->bodyImage!=NULL)
            {
                // Show the Object body on the world
                //Apply the image stretched
				SDL_Rect renderQuad;
				renderQuad.x = gridSize.second*(*it)->y;
				renderQuad.y = gridSize.first*(*it)->x;
				renderQuad.w = gridSize.second;
				renderQuad.h = gridSize.first;
				//Render to screen
                SDL_RenderCopy( renderer, (*it)->bodyImage, NULL, &renderQuad );
            } else {
                SDL_Color color = (*it)->color;
                SDL_SetRenderDrawColor( renderer, color.r, color.b, color.g, color.a);
                fillRect = { gridSize.second*(*it)->y, gridSize.first*(*it)->x, gridSize.second, gridSize.first };
                SDL_RenderFillRect( renderer, &fillRect );
            }
        }

        // Update the world map for next cycle
        updateWorldMap();

        // Allow the world to execute it's own code during the process
        execute();

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
        // Time (dt) between frames **No need for fps**
        time = (SDL_GetTicks() - delta);
        //std::cout<<time<<std::endl;
        //Offset time computing with delay time for making it slower
        SDL_Delay(fdim(timeSpeed,time));
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
    if(grid[x][y]!=OPENSPACE && grid[x][y]!=DOORWAY)
        return false;
    for(std::vector<Object*>::iterator it=objects.begin(); it!= objects.end(); it++)
    {
        // Does not check if the same object because if
        // object does not move then object move() returns true
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

void World::removeObject(Object* temp)
{
    std::vector<Object*>::iterator it = std::find (objects.begin(), objects.end(), temp);
    objects.erase(it);
}

void World::updateWorldMap()
{
    worldMap = gridMap;
    Map::MultiArray& wMap = worldMap.getOpenGrid();
    for(std::vector<Object*>::iterator it=objects.begin(); it!= objects.end(); it++)
    {
        wMap[(*it)->x][(*it)->y] = (*it)->id;
    }
}
