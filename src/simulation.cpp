#include "simulation.h"

std::mutex out;

using namespace std;

bool Simulation::simulationV8(SimulationData simulationData, int ti){

	simulationData.isAReuser = !(simulationData.reuseBonusMultiplierA == -1.0);
	simulationData.isBReuser = !(simulationData.reuseBonusMultiplierB == -1.0);
	simulationData.isAProducer = !(simulationData.buildingBonusA == -1.0);
	simulationData.isBProducer = !(simulationData.buildingBonusB == -1.0);

	SimulationResults simulationResults = SimulationRun::runSimV8(simulationData, ti);

	out.lock();
	if(simulationData.behaviorHistory){
		cout<<simulationResults.behaviorHistoryStr;
	}
	if(simulationData.ephPopHistory){
		for(unsigned int i = 0; i < simulationResults.typeAPopHistory.size(); i++){
			cout<<i<<";"<<simulationResults.typeAPopHistory[i]<<";"<<simulationResults.typeBPopHistory[i]<<";"<<simulationResults.ephPopHistory[i]<<";"<<simulationResults.avgDegree[i]<<endl;
		}
	}else{

		if(simulationData.printPartials){
			for(unsigned int i = 500; i <= simulationResults.typeAPopHistory.size(); i+=500){
				cout<<"partial;"<<i<<";"<<simulationResults.typeAPopHistory[i-1]<<";"<<simulationResults.typeBPopHistory[i-1]<<";"<<simulationResults.ephPopHistory[i-1]<<";"<<simulationData.sampleid<<endl;
			}
		}

		int fc = simulationResults.fixationCycles;
		int numCycles = simulationResults.typeAPopHistory.size()-1;
		cout<<simulationResults.typeAPopHistory[numCycles]<<";"<<simulationResults.typeBPopHistory[numCycles]<<";"<<fc<<";"<<simulationResults.elapsedSeconds<<";"<<simulationData.sampleid<<";"<<simulationResults.typeAWithEph<<";"<<simulationResults.typeBWithEph<<endl;
	}

	out.unlock();

	return true;
}

bool Simulation::simulationV7(SimulationData simulationData, int ti){

	SimulationResults simulationResults = SimulationRun::runSimV7(simulationData, ti);

	out.lock();
	if(simulationData.ephPopHistory){
		for(unsigned int i = 0; i < simulationResults.typeAPopHistory.size(); i++){
			cout<<i<<";"<<simulationResults.typeAPopHistory[i]<<";"<<simulationResults.typeBPopHistory[i]<<";"<<simulationResults.ephPopHistory[i]<<";"<<simulationResults.avgDegree[i]<<endl;
		}
	}else{
		int fc = simulationResults.fixationCycles;
		int numCycles = simulationResults.typeAPopHistory.size()-1;

		cout<<simulationResults.typeAPopHistory[numCycles]<<";"<<simulationResults.typeBPopHistory[numCycles]<<";"<<fc<<";"<<simulationResults.elapsedSeconds<<";"<<simulationData.sampleid<<";"<<simulationResults.typeAWithEph<<";"<<simulationResults.typeBWithEph<<endl;
	}

	out.unlock();

	return true;
}

vector<SimulationResults> Simulation::simResults;
