#ifndef MODEL_SIMULATIONDATA
#define MODEL_SIMULATIONDATA

#include <vector>
#include <string>

class GsopGraph;

class SimulationData{

	public:

	int initialPop;
	double birthRate;
	double deathRate;
	char type;
	int cycles;
	int plotDensity;
	double ephBonus;
	double ephBonusB;
	double ephStartRatio;
	double ephBuildingRatio;
	double ephReusingRatio;
	double ephBirthGenerationChance;
	bool neighborhoodInheritance;
	bool aOnly;
	bool ephPopHistory;
	bool behaviorHistory;
	int ephTime;
	int behaviorTime;
	int buildingTime;
	double reuseBonusMultiplierA;
	double reuseBonusMultiplierB;
	double buildingBonusA;
	double buildingBonusB;
	double searchingBonusA;
	double searchingBonusB;
	bool isAProducer;
	bool isBProducer;
	bool isAReuser;
	bool isBReuser;
	bool bEph;
	double cheaterChanceA;
	double cheaterChanceB;
	bool printPartials;
	bool lockABEph;

	int sampleid;
	std::vector<std::string> nodeDetail;
	GsopGraph *g;

};

#endif