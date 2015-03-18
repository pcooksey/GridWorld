#include "map.h"

Map::Map()
{

}

Map::~Map()
{
    //dtor
}

bool Map::open(std::string fileName)
{
    std::ifstream file;
    file.open(fileName.c_str());
    if(file.fail())
        return false;
    // This is read x value then y value just like SDL reads it
    file>>height>>width;
    grid.resize(height);
    for (int i = 0; i < height; ++i)
        grid[i].resize(width);
    int value(0);
    for(int x=0; x<height; x++)
        for(int y=0; y<width; y++)
        {
            file>>value;
            grid[x][y] = value;
        }
    return true;
}
