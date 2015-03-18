#include "gridsearch.h"

/** Initialization of const static variables */
const int GridSearch::NoID = -1;
const int GridSearch::NullID = -2;
const GridSearch::Node GridSearch::NullNode = GridSearch::Node(-1,-1);

Rules::Rules()
{

}

Rules::~Rules()
{

}

bool Rules::check(GridSearch* searcher, int id)
{
    if(id==0 || id==3)
        return true;
    return false;
}

GridSearch::GridSearch(const Map& grid, Rules* rules)
 :grid(grid),rules(rules)
{
    //ctor
}

GridSearch::~GridSearch()
{
    //dtor
}

GridSearch::Path GridSearch::BFS(Node start, Node goal, int id)
{
    Path nodes;
    std::queue<Node > Q;
    std::set<Node > discovered;
    std::map<Node, Node> parentsMap;
    Q.push(start);
    discovered.insert(start);
    Node nully = Node(-1,-1);
    parentsMap[start] = nully;
    while(!Q.empty())
    {
        Node node = Q.front();
        Q.pop();
        std::cout<<nodeID(node)<<std::endl;
        if(node==goal || nodeID(node)==id)
        {
            std::cout<<"Here already";
            while(node!=nully)
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

GridSearch::Path GridSearch::getBranches(Node node)
{
    Path nodes;
    int x=node.first, y=node.second;
    // Directions don't matter since this is graph searching so names are meaningless
    Node top(x-1,y), bottom(x+1,y), left(x,y+1), right(x,y-1);
    if(rules->check(this, nodeID(top)))
        nodes.push_back(top);
    if(rules->check(this, nodeID(bottom)))
        nodes.push_back(bottom);
    if(rules->check(this, nodeID(left)))
        nodes.push_back(left);
    if(rules->check(this, nodeID(right)))
        nodes.push_back(right);
    std::random_shuffle ( nodes.begin(), nodes.end() );
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
