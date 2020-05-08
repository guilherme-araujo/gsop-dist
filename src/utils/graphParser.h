#ifndef UTILS_GRAPHPARSER
#define UTILS_GRAPHPARSER

#include <string>

#include "../model/Graph.h"

class Parser {
public:
    GsopGraph * parse(std::string file);

};

#endif