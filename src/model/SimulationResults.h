#ifndef MODEL_SIMULATIONRESULTS
#define MODEL_SIMULATIONRESULTS

#include <vector>
#include "GsopNode.h"

class SimulationResults{

	public:

	std::vector<int> typeAPopHistory;
	std::vector<int> typeBPopHistory;
	std::vector<int> ephPopHistory;
	std::vector<int> avgDegree;
	int typeAWithEph;
	int typeBWithEph;
	int typeACheater;
	int typeBCheater;
	std::vector<double> partialFitnessAvg;
	std::vector<GsopNode> finalNodes;
	double elapsedSeconds;
	double avgCoeff;
	double avgFitness;
	int fixationCycles;
	std::string behaviorHistoryStr;


};

#endif