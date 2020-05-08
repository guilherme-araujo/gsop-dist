#include <iostream>
#include <unistd.h>
#include <thread>
#include <future>
#include "utils/graphParser.h"
#include "simulation.h"

using namespace std;

void waitupdate(){
	usleep(100000);
}

int main(int argc, char* argv[]){

	//Default simulation values
	int samples = 1;
	int cycles = 1000;
	int numNodes = 100;
	double ephBonus = 0.04;
	double ephBonusB = -1;
	int threads = 1;
	double ephStartRatio = 0.6;
	double ephBuildingRatio = 0.3333;
	double ephReusingRatio = 0.3333;
	bool ephPopHistory = false;
	bool behaviorHistory = false;
	int ephTime = 30;
	bool ni = false;
	int sampleid = 0;
	char graphType = 'r';
	bool bEph = 1;
	int behaviorTime = 30;
	double reuseBonusMultiplierA = 0.3;
	double reuseBonusMultiplierB = 0.9;
	double buildingBonusA = 0.99;
	double buildingBonusB = 0.93;
	bool printPartials = false;

	//Simulation values parsing from argv
	string arg_samples = "samples";
	string arg_cycles = "cycles";
	string arg_numNodes = "numNodes";
	string arg_ephBonus = "ephBonus";
	string arg_ephBonusB = "ephBonusB";
	string arg_threads = "threads";
	string arg_ephStartRatio = "ephStartRatio";
	string arg_ephBuildingRatio = "ephBuildingRatio";
	string arg_ephReusingRatio = "ephReusingRatio";
	string arg_ephPopHistory = "ephPopHistory";
	string arg_behaviorHistory = "behaviorHistory";
	string arg_ephTime = "ephTime";
	string arg_ni = "ni";
	string arg_sampleid = "sampleId";
	string arg_graphtype = "graphType";
	string arg_bEph = "bEph";
	string arg_behaviorTime = "behaviorTime";
	string arg_reuseBonusMultiplierA = "rBMA";
	string arg_reuseBonusMultiplierB = "rBMB";
	string arg_buildingBonusA = "bBA";
	string arg_buildingBonusB = "bBB";
	string arg_printPartials = "printPartials";

	for(int i = 1; i < argc; i+=2){
		if(arg_samples.compare(argv[i])==0){
			samples = stoi(argv[i+1]);
		}else if(arg_cycles.compare(argv[i])==0){
			cycles = stoi(argv[i+1]);
		}else if(arg_numNodes.compare(argv[i])==0){
			numNodes = stoi(argv[i+1]);
		}else if(arg_ephBonus.compare(argv[i])==0){
			ephBonus = stod(argv[i+1]);
		}else if(arg_ephBonusB.compare(argv[i])==0){
			ephBonusB = stod(argv[i+1]);
		}else if(arg_threads.compare(argv[i])==0){
			threads = stoi(argv[i+1]);
		}else if(arg_ephStartRatio.compare(argv[i])==0){
			ephStartRatio = stod(argv[i+1]);
		}else if(arg_ephBuildingRatio.compare(argv[i])==0){
			ephBuildingRatio = stod(argv[i+1]);
		}else if(arg_ephReusingRatio.compare(argv[i])==0){
			ephReusingRatio = stod(argv[i+1]);
		}else if(arg_ephPopHistory.compare(argv[i])==0){
			ephPopHistory = stoi(argv[i+1]);
		}else if(arg_ephTime.compare(argv[i])==0){
			ephTime = stoi(argv[i+1]);
		}else if(arg_ni.compare(argv[i])==0){
			ni = stoi(argv[i+1]);
		}else if (arg_sampleid.compare(argv[i])==0){
			sampleid = stoi(argv[i+1]);
		}else if (arg_graphtype.compare(argv[i])==0){
			graphType = argv[i+1][0];
		}else if(arg_bEph.compare(argv[i])==0){
			bEph = stoi(argv[i+1]);
		}else if (arg_behaviorTime.compare(argv[i])==0){
			behaviorTime = stoi(argv[i+1]);
		}else if (arg_reuseBonusMultiplierA.compare(argv[i])==0){
			reuseBonusMultiplierA = stod(argv[i+1]);
		}else if (arg_reuseBonusMultiplierB.compare(argv[i])==0){
			reuseBonusMultiplierB = stod(argv[i+1]);
		}else if (arg_buildingBonusA.compare(argv[i])==0){
			buildingBonusA = stod(argv[i+1]);
		}else if (arg_buildingBonusB.compare(argv[i])==0){
			buildingBonusB = stod(argv[i+1]);
		}else if (arg_printPartials.compare(argv[i])==0){
			printPartials = stoi(argv[i+1]);
		}else if (arg_behaviorHistory.compare(argv[i])==0){
			behaviorHistory = stoi(argv[i+1]);
		}else{
			cout<<"Wrong option "<<argv[i]<<endl;
		}

	}
	
	//Parse graph generated by python script or another source
	Parser p;
	GsopGraph *g = p.parse("graph.txt");
	
	if(ephBonusB==-1){
		ephBonusB = ephBonus;
	}
	
	SimulationData simulationData;

	simulationData.ephBonus = ephBonus;
	simulationData.ephBonusB = ephBonusB;
	simulationData.ephStartRatio = ephStartRatio;
	simulationData.ephBuildingRatio = ephBuildingRatio;
	simulationData.ephReusingRatio = ephReusingRatio;
	simulationData.ephBirthGenerationChance = 0.5;
	simulationData.aOnly = false;
	simulationData.neighborhoodInheritance = ni;
	simulationData.birthRate = 1.04;
	simulationData.deathRate = 1.04;
	simulationData.plotDensity = 100;
	simulationData.cycles = cycles;
	simulationData.initialPop = g->nodes.size();
	simulationData.ephTime = ephTime;
	simulationData.g = g;
	simulationData.ephPopHistory = ephPopHistory;
	simulationData.sampleid = sampleid;
	simulationData.graphType = graphType;
	simulationData.bEph = bEph;
	simulationData.behaviorTime = behaviorTime;
	simulationData.reuseBonusMultiplierA = reuseBonusMultiplierA;
	simulationData.reuseBonusMultiplierB = reuseBonusMultiplierB;
	simulationData.buildingBonusA = buildingBonusA;
	simulationData.buildingBonusB = buildingBonusB;
	simulationData.printPartials = printPartials;
	simulationData.behaviorHistory = behaviorHistory;

	//Launch simulation threads according to number of samples
	vector<thread> tl;
	tl.resize(threads);
	vector<bool> tb; //false as not running
	tb.resize(threads);

	int scount = 0;

	vector< future<bool> >fut;
	fut.resize(threads);

	while(true){
		int ti = 0;

		for(ti = 0; ti < threads; ti++){

			if(!tb[ti] && scount!=samples){
				simulationData.sampleid = scount++;

				fut[ti] = async(Simulation::simulationV8,simulationData, ti);

				tb[ti] = true;
				
			}

			if(tb[ti]==true){
				if(fut[ti].wait_for(chrono::seconds(0))==future_status::ready){
					fut[ti].get();
					tb[ti] = false;
				}

			}

		}
		if(scount==samples){
			bool allover = true;
			for(ti = 0; ti < threads; ti++){
				if(tb[ti]){
					allover = false;
					break;
				}
			}
			if(allover) break;
		}

		thread wait = thread(waitupdate);
		wait.join();

	}

    return 0;
}