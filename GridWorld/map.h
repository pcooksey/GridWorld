#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <vector>
#include <string>

class Map
{
    public:
        typedef std::pair<int, int> MapSize;
        typedef std::vector<std::vector<int> > MultiArray;
        Map();
        virtual ~Map();
        bool open(std::string fileName);

        MapSize getSize() const { return MapSize(height,width);};

        const MultiArray& getGrid() const { return grid; };

        MultiArray& getOpenGrid() { return grid; };

    protected:
    private:
        MultiArray grid;
        int width;
        int height;
};

#endif // MAP_H
