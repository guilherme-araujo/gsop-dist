#include "simulationRun.h"

using namespace std;

SimulationResults SimulationRun::runSimV8(SimulationData simulationData, int ti){

	SimulationResults simulationResults;

	simulationResults.behaviorHistoryStr = "";
	simulationResults.fixationCycles = -1;

	vector<GsopNode> nodes = simulationData.g->nodes;

	srand(time(NULL));
	random_device rd;
	array<int, std::mt19937::state_size> seed_data;
	generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
	seed_seq seq(std::begin(seed_data), std::end(seed_data));
	mt19937 eng(seq);
	shuffle(nodes.begin(), nodes.end(), eng);

	unordered_map<int, GsopNode> nodesmap;
	simulationData.aOnly = false;
	simulationData.bEph = true;
	simulationData.neighborhoodInheritance = false; //V8 simulations are always without neighborhood inheritance

	for(int i = 0; i < nodes.size(); i++){

		double abrate = 0.5;
		if(simulationData.aOnly){
			abrate = 1;
		}

		if(i < simulationData.initialPop*abrate){
			//Type A initialization
			nodes[i].type = 'A';
			nodes[i].coeff = 1.0;
			int aEphIndex = simulationData.initialPop * abrate * simulationData.ephStartRatio;
			//separates state using from producing
			int aEphIndexBuilding = aEphIndex * simulationData.ephBuildingRatio;
			int aEphIndexUsingShared = (aEphIndex * simulationData.ephReusingRatio) + aEphIndexBuilding;

			if ((i < aEphIndex) && simulationData.isAProducer) {
				if(i < aEphIndexBuilding){
					Eph *e = new Eph(simulationData.ephBonus);

					//randomize eph time
					uniform_int_distribution<> distr_eph(1, simulationData.ephTime);
					int eTime = distr_eph(eng);
					e->time = eTime;

					nodes[i].eph = e;
					nodes[i].behavior = PRODUCING;
				} else if(i < aEphIndexUsingShared){
					Eph *e = new Eph(simulationData.ephBonus);

					//randomize eph time, -1 because reused ephs have had at least 1 cycle with previous owner
					uniform_int_distribution<> distr_eph(1, simulationData.ephTime-1);
					int eTime = distr_eph(eng);
					e->time = eTime;

					nodes[i].eph = e;
					nodes[i].behavior = USING_SHARED;
				} else{
					nodes[i].eph = NULL;
					nodes[i].behavior = USING;
				}
			}else{
				nodes[i].eph = NULL;
				nodes[i].behavior = SEARCHING;
			}

			if(nodes[i].behavior != USING && nodes[i].behavior != USING_SHARED){
				//randomize behavior time
				uniform_int_distribution<> distr_behavior(1, simulationData.behaviorTime);
				int sTime = distr_behavior(eng);
				nodes[i].behaviorTimer = sTime;
			}else{
				nodes[i].behaviorTimer = 0;
			}
		} else{
			//Type B initialization
			nodes[i].type = 'B';
			nodes[i].coeff = 1.0;
			int bEphIndex = (simulationData.initialPop * abrate * simulationData.ephStartRatio)+(simulationData.initialPop * abrate);
			//separates state using from producing
			int bEphIndexBuilding = (bEphIndex - simulationData.initialPop * abrate) * simulationData.ephBuildingRatio + (simulationData.initialPop * abrate);
			int bEphIndexUsingShared = ((bEphIndex - simulationData.initialPop * abrate) * simulationData.ephReusingRatio) + bEphIndexBuilding;
			if (i < bEphIndex && simulationData.isBProducer) {
				if(i < bEphIndexBuilding){
					Eph *e = new Eph(simulationData.ephBonus);

					//randomize eph time
					uniform_int_distribution<> distr_eph(1, simulationData.ephTime);
					int eTime = distr_eph(eng);
					e->time = eTime;
					nodes[i].eph = e;
					nodes[i].behavior = PRODUCING;
				}else if(i < bEphIndexUsingShared){
					Eph *e = new Eph(simulationData.ephBonus);

					//randomize eph time
					uniform_int_distribution<> distr_eph(1, simulationData.ephTime-1);
					int eTime = distr_eph(eng);
					e->time = eTime;
					nodes[i].eph = e;
					nodes[i].behavior = USING_SHARED;
				}else{
					nodes[i].eph = NULL;
					nodes[i].behavior = USING;
				}
			} else{
				nodes[i].eph = NULL;
				nodes[i].behavior = SEARCHING;
			}

			if(nodes[i].behavior != USING && nodes[i].behavior != USING_SHARED){
				//randomize behavior time
				uniform_int_distribution<> distr_behavior(1, simulationData.behaviorTime);
				int sTime = distr_behavior(eng);
				nodes[i].behaviorTimer = sTime;
			}else {
				nodes[i].behaviorTimer = 0;
			}
		}
		nodesmap.insert(pair<int,GsopNode>(nodes[i].id,nodes[i]));
	}

	for(int i = 0; i < nodes.size(); i++){
		for(int j = 0; j < nodes[i].neighbors.size(); j++){
			nodesmap[nodes[i].neighbors[j]].neighbors.push_back(nodes[i].id);
		}
	}

	clock_t begin = clock();

	for(int i = 0; i < simulationData.cycles; i++){
		string behaviorSnapshot;
		SimulationCycles::cycleV8(&nodesmap, simulationData, &eng, &behaviorSnapshot);

		int ephCount = 0;
		int typeACount = 0;
		int typeBCount = 0;

		vector<GsopNode> nodeslist;

		int degreeCount = 0;
		for(unordered_map<int, GsopNode>::iterator j = nodesmap.begin(); j != nodesmap.end(); ++j){
			if(j->second.eph != NULL){
				ephCount++;
			}
			if(j->second.type == 'A'){
				typeACount++;
			}else{
				typeBCount++;
			}
			degreeCount += j->second.neighbors.size();

		}

		if(simulationData.behaviorHistory){
			simulationResults.behaviorHistoryStr += "bhistory;"+to_string(simulationData.sampleid)+
			';'+to_string(i)+';'+behaviorSnapshot;

		}

		simulationResults.avgDegree.push_back(degreeCount/nodes.size());
		simulationResults.ephPopHistory.push_back(ephCount);
		simulationResults.typeAPopHistory.push_back(typeACount);
		simulationResults.typeBPopHistory.push_back(typeBCount);

		if(typeACount == 0 || typeBCount == 0){
			simulationResults.fixationCycles = i;
			break;
		}

		

	}

	int typeACount = 0;
	int typeBCount = 0;

	for(unordered_map<int, GsopNode>::iterator j = nodesmap.begin(); j != nodesmap.end(); ++j){
		if(j->second.eph != NULL){
			if(j->second.type == 'A'){
				typeACount++;
			}else{
				typeBCount++;
			}
		}
	}

	simulationResults.typeAWithEph = typeACount;
	simulationResults.typeBWithEph = typeBCount;

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	simulationResults.elapsedSeconds = elapsed_secs;

	return simulationResults;
}

SimulationResults SimulationRun::runSimV7(SimulationData simulationData, int ti){
	SimulationResults simulationResults;

	simulationResults.fixationCycles = -1;

	vector<GsopNode> nodes = simulationData.g->nodes;

	srand(time(NULL));
	random_device rd;
	array<int, std::mt19937::state_size> seed_data;
	generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
	seed_seq seq(std::begin(seed_data), std::end(seed_data));
	mt19937 eng(seq);
	shuffle(nodes.begin(), nodes.end(), eng);

	unordered_map<int, GsopNode> nodesmap;

	for(int i = 0; i < nodes.size(); i++){
		double abrate = 0.5;
		if(simulationData.aOnly){
			abrate = 1;
		}
		if(i < simulationData.initialPop*abrate){
			nodes[i].type = 'A';
			nodes[i].coeff = 1.0;
			if (i < simulationData.initialPop * abrate * simulationData.ephStartRatio) {
				Eph *e = new Eph(simulationData.ephBonus);
				e->time = simulationData.ephTime;
				nodes[i].eph = e;
			}else{
				nodes[i].eph = NULL;
			}
		} else{
			nodes[i].type = 'B';
			nodes[i].coeff = 1.0;
			if(simulationData.bEph){
				if (i < (simulationData.initialPop * abrate * simulationData.ephStartRatio)+(simulationData.initialPop * abrate) ) {
					Eph *e = new Eph(simulationData.ephBonus);
					e->time = simulationData.ephTime;
					nodes[i].eph = e;
				}else{
					nodes[i].eph = NULL;
				}
			}else{
				nodes[i].eph = NULL;
			}
		}
		nodesmap.insert(pair<int,GsopNode>(nodes[i].id,nodes[i]));
	}


	for(int i = 0; i < nodes.size(); i++){
		for(int j = 0; j < nodes[i].neighbors.size(); j++){
			nodesmap[nodes[i].neighbors[j]].neighbors.push_back(nodes[i].id);
		}
	}

	clock_t begin = clock();



	for(int i = 0; i < simulationData.cycles; i++){
		SimulationCycles::cycleV7(&nodesmap, simulationData, &eng);

		int ephCount = 0;
		int typeACount = 0;
		int typeBCount = 0;

		vector<GsopNode> nodeslist;

		int degreeCount = 0;
		for(unordered_map<int, GsopNode>::iterator j = nodesmap.begin(); j != nodesmap.end(); ++j){
			if(j->second.eph != NULL){
				ephCount++;
			}
			if(j->second.type == 'A'){
				typeACount++;
			}else{
				typeBCount++;
			}
			degreeCount += j->second.neighbors.size();

		}

		simulationResults.avgDegree.push_back(degreeCount/nodes.size());
		simulationResults.ephPopHistory.push_back(ephCount);
		simulationResults.typeAPopHistory.push_back(typeACount);
		simulationResults.typeBPopHistory.push_back(typeBCount);

		if(typeACount == 0 || typeBCount == 0){
			simulationResults.fixationCycles = i;
			break;
		}

	}
	int typeACount = 0;
	int typeBCount = 0;

	for(unordered_map<int, GsopNode>::iterator j = nodesmap.begin(); j != nodesmap.end(); ++j){
		if(j->second.eph != NULL){
			if(j->second.type == 'A'){
				typeACount++;
			}else{
				typeBCount++;
			}
		}
	}

	simulationResults.typeAWithEph = typeACount;
	simulationResults.typeBWithEph = typeBCount;

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


	simulationResults.elapsedSeconds = elapsed_secs;

	return simulationResults;
}
