#ifndef GRIDSEARCH_H
#define GRIDSEARCH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <map>

#include "map.h"

class GridSearch;

class Rules {
    public:
        Rules();
        virtual ~Rules();
        /** Simple @World based search which only approves open spaces */
        virtual bool check(GridSearch* searcher, int id);
    protected:

        friend class GridSearch;
};

class GridSearch
{
    public:
        typedef std::pair<int, int> Node;
        typedef std::vector<Node > Path;
        /** static variable for NoID */
        const static int NoID;
        const static int NullID;
        const static Node NullNode;

        GridSearch(const Map& grid, Rules* rules);
        virtual ~GridSearch();

        /** Breath-first search returns a path stored in vector> */
        Path BFS(Node start, Node goal = NullNode, int id = NullID);

    protected:
        /** Returns all branches */
        Path getBranches(Node node);
        /** Returns the @id of the @node */
        int nodeID(Node node);

    private:
        const Map& grid;
        Rules* rules;
};

#endif // GRIDSEARCH_H
