#ifndef MODEL_GRAPH
#define MODEL_GRAPH

#include <vector>
#include "GsopNode.h"

class GsopGraph{
public:
    std::vector<GsopNode> nodes;
    std::vector< std::vector<int> > adjMatrix;

    int size();
};

#endif