#ifndef SIMULATIONCYCLES_CLASS
#define SIMULATIONCYCLES_CLASS

#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <map>
#include <algorithm>
#include "model/GsopNode.h"

class SimulationCycles{

public:

    static void cycleV8(std::unordered_map<int,GsopNode> *nodes, SimulationData simulationData, std::mt19937 *eng, std::string *behaviorSnapshot);
    static void cycleV7(std::unordered_map<int,GsopNode> *nodes, SimulationData simulationData, std::mt19937 *eng);

};

#endif
