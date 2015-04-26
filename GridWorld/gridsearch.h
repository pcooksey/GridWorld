#ifndef GRIDSEARCH_H
#define GRIDSEARCH_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <set>
#include <map>

#include "map.h"

class GridSearch;

class Rules {
    public:
        typedef std::pair<int, int> Node;
        typedef std::vector<int> IDs;

        Rules();
        virtual ~Rules();
        /** Simple @World based search which only approves open spaces and doorways */
        virtual bool check(GridSearch* searcher, int id);
    protected:

        /** Gets the @start node for the search */
        Node getStart(GridSearch* searcher);
        /** Gets the @goal node for the search */
        Node getGoal(GridSearch* searcher);
        /** Gets the @current node for the search */
        Node getCurrent(GridSearch* searcher);
        /** Gets the @checking node for the search */
        Node getChecking(GridSearch* searcher);
        /** Gets the @id for the search */
        int getId(GridSearch* searcher);
        /** Gets the @id of the branches to a node */
        IDs getBranchesIDs(GridSearch* searcher, Node node);
};

class GridSearch
{
    public:
        typedef std::pair<int, int> Node;
        typedef std::vector<Node > Path;
        typedef std::vector<int> IDs;
        /** static variable for NoID */
        const static int NoID;
        const static int NullID;
        const static Node NullNode;

        GridSearch(const Map& grid, Rules* rules);
        virtual ~GridSearch();

        /** Breath-first search returns a path stored in vector */
        Path BFS(Node start, Node goal = NullNode, int id = NullID);
        /** Depth-first search returns a path stored in vector */
        Path DFS(Node start, Node goal = NullNode, int id = NullID);

    protected:
        /** Returns all branches with checks from rules */
        Path getBranches(Node node);
        /** Returns all branches ids (to be used by rules) */
        IDs getBranchesIDs(Node node);
        /** Returns the @id of the @node */
        int nodeID(Node node);

        /** These values are set by search functions and can be used
            by the Rules class to determine case based reasoning */
        Node start;
        Node goal;
        Node current;
        Node checking;
        int id;

    private:
        const Map& grid;
        Rules* rules;

        friend class Rules;
};

#endif // GRIDSEARCH_H
