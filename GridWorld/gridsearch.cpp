#include "gridsearch.h"

/** Initialization of const static variables */
const int GridSearch::NoID = -1;
const int GridSearch::NullID = -2;
const GridSearch::Node GridSearch::NullNode = GridSearch::Node(-1,-1);

/**
*
*    Rule class functions
*
*/

Rules::Rules()
{

}

Rules::~Rules()
{

}

bool Rules::check(GridSearch* searcher, int id)
{
    //These numbers are just static for the world even though
    //the class does not relate to the world. Easier for testing default
    //then having a pure function
    if(id==0 || id==3)
        return true;
    return false;
}

Rules::Node Rules::getStart(GridSearch* searcher)
{ return searcher->start; }

Rules::Node Rules::getGoal(GridSearch* searcher)
{ return searcher->goal; }

Rules::Node Rules::getCurrent(GridSearch* searcher)
{ return searcher->current; }

Rules::Node Rules::getChecking(GridSearch* searcher)
{ return searcher->checking; }

int Rules::getId(GridSearch* searcher)
{ return searcher->id; }

Rules::IDs Rules::getBranchesIDs(GridSearch* searcher, Node node)
{ return searcher->getBranchesIDs(node); }

/**
*
*    Grid search class functions
*
*/

GridSearch::GridSearch(const Map& grid, Rules* rules)
 :grid(grid),rules(rules)
{
    start = NullNode;
    goal = NullNode;
    current = NullNode;
    checking = NullNode;
    id = NullID;
}

GridSearch::~GridSearch()
{
    //dtor
}

GridSearch::Path GridSearch::BFS(Node start, Node goal, int id)
{
    this->start = start;
    this->goal = goal;
    this->id = id;
    Path nodes;
    std::queue<Node > Q;
    std::set<Node > discovered;
    std::map<Node, Node> parentsMap;
    Q.push(start);
    discovered.insert(start);
    parentsMap[start] = NullNode;
    Node node = NullNode;
    while(!Q.empty())
    {
        node = Q.front();
        this->current = node;
        Q.pop();
        if(node==goal || nodeID(node)==id)
        {
            while(node!=NullNode)
            {
                nodes.push_back(node);
                node = parentsMap[node];
            }
            std::reverse(nodes.begin(),nodes.end());
            return nodes;
        }
        Path edges = getBranches(node);
        for(Path::iterator it=edges.begin(); it!=edges.end(); it++)
        {
            if(discovered.find((*it)) == discovered.end())
            {
                parentsMap[(*it)] = node;
                Q.push((*it));
                discovered.insert((*it));
            }
        }
    }
    return nodes;
}

GridSearch::Path GridSearch::DFS(Node start, Node goal, int id)
{
    this->start = start;
    this->goal = goal;
    this->id = id;
    Path nodes;
    std::stack<Node > Q;
    std::set<Node > discovered;
    std::map<Node, Node> parentsMap;
    parentsMap[start] = NullNode;
    Q.push(start);
    Node node = NullNode;
    while(!Q.empty())
    {
        node = Q.top();
        this->current = node;
        Q.pop();
        if(node==goal || nodeID(node)==id)
        {
            while(node!=NullNode)
            {
                nodes.push_back(node);
                node = parentsMap[node];
            }
            std::reverse(nodes.begin(),nodes.end());
            return nodes;
        }
        if(discovered.find(node) == discovered.end())
        {
            discovered.insert(node);
            Path edges = getBranches(node);
            for(Path::iterator it=edges.begin(); it!=edges.end(); it++)
            {
                if(parentsMap.find((*it))==parentsMap.end())
                    parentsMap[(*it)] = node;
                Q.push((*it));
            }
        }
    }
    return nodes;
}

GridSearch::Path GridSearch::getBranches(Node node)
{
    Path nodes;
    int x=node.first, y=node.second;
    // Directions don't matter since this is graph searching so names are meaningless
    Node top(x-1,y), bottom(x+1,y), left(x,y+1), right(x,y-1);
    checking=top;
    if(rules->check(this, nodeID(top)))
        nodes.push_back(top);
    checking=bottom;
    if(rules->check(this, nodeID(bottom)))
        nodes.push_back(bottom);
    checking=left;
    if(rules->check(this, nodeID(left)))
        nodes.push_back(left);
    checking=right;
    if(rules->check(this, nodeID(right)))
        nodes.push_back(right);
    std::random_shuffle ( nodes.begin(), nodes.end());
    return nodes;
}

GridSearch::IDs GridSearch::getBranchesIDs(Node node)
{
    IDs nodes;
    int x=node.first, y=node.second;
    // Directions don't matter since this is graph searching so names are meaningless
    Node top(x-1,y), bottom(x+1,y), left(x,y+1), right(x,y-1);
    if(nodeID(top)!=NoID)
        nodes.push_back(nodeID(top));
    if(nodeID(bottom)!=NoID)
        nodes.push_back(nodeID(bottom));
    if(nodeID(left)!=NoID)
        nodes.push_back(nodeID(left));
    if(nodeID(right)!=NoID)
        nodes.push_back(nodeID(right));
    return nodes;
}

int GridSearch::nodeID(Node node)
{
    Map::MapSize mapSize = grid.getSize();
    const Map::MultiArray& gridMap = grid.getGrid();
    if(node.first<0 || node.second<0 ||
       node.first>=mapSize.first || node.second>=mapSize.second)
       return NoID;
    else {
        return gridMap[node.first][node.second];
    }
}
