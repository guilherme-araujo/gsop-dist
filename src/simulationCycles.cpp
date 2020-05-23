#include "simulationCycles.h"

using namespace std;

void SimulationCycles::cycleV8(unordered_map<int,GsopNode> *nodes, SimulationData simulationData, mt19937 *eng, string *behaviorSnapshot){

	//count deaths nascimentos
	int dieCount = (int) ((double) nodes->size() * (simulationData.deathRate-1) );

	//make sure there is at least one death
	if(dieCount==0) dieCount++;

	//vector of selected keys
	vector<int> selectedKeys;

	//vector of selected keys for death in case of neighborhood inheritance
	vector<int> selectedKeysDeath;

	//all nodes key vector
	vector<int> keys;
	keys.resize(nodes->size());
	for(int i = 0; i < nodes->size(); i++){
		keys[i] = i;
	}
	shuffle(keys.begin(), keys.end(), *eng);

	//deaths
	for(int i = 0; i < dieCount; i++){
		selectedKeys.push_back(keys[i]);
	}

	//births
	//keyi: current selected key index for death/birth
	for(int keyi = 0; keyi < selectedKeys.size(); keyi++){

		//key: current selected key
		int key = selectedKeys[keyi];

		//*n: current selected node
		GsopNode *n = &(*nodes)[key];

		//selected type. saving for reuse of eph only by same type
		char selectedType = n->type;

		vector<double> roulette;
		vector<int> neighborsList = n->neighbors;
		double qtd = 0;
		for(int i = 0; i < neighborsList.size(); i++){
			GsopNode neighbor = (*nodes)[neighborsList[i]];

			//adjusted roulette amounts by node state			
			qtd += neighbor.getCoeffV8(simulationData);
			//for(int j = 0; j < qtd; j++){
			roulette.push_back(qtd);
			
		}

		Eph *eph = NULL;
		GsopNode *sorteado;

		if(roulette.size()==0){
			sorteado = &(*nodes)[key];
		}else if(roulette.size()==1){
			sorteado = &(*nodes)[neighborsList[0]];
		}else {
			int iSorteado = 0;
			uniform_real_distribution<> distr(0, roulette[roulette.size()-1]);
			double chosen = distr(*eng);//rand()%roulette.size();
			
			int l = 1; int r = roulette.size()-1;
			while(l<=r){
				int m = ((r-l)/2)+l;
				double nl = roulette[m-1];
				double nr = roulette[m];
				if(chosen>nl && chosen<=nr){
					iSorteado = m;
					break;
				}
				if(chosen>nr){
					l = m+1;
				}else{
					r = m-1;
				}

			}

			int sorteadoIndex = neighborsList[iSorteado];
			sorteado = &(*nodes)[sorteadoIndex];
		}

		sorteado->fitness++;

		if(!simulationData.neighborhoodInheritance){
			eph = (n->eph);
			
			n->coeff = sorteado->coeff;
			n->type = sorteado->type;
			n->fitness = 0;
			if((simulationData.bEph || n->type=='A') && simulationData.ephBirthGenerationChance > 0.0){
				uniform_real_distribution<> distr(0, 1);
				double chosen = distr(*eng);
				if(chosen < simulationData.ephBirthGenerationChance){
					n->behavior = USING;
					Eph *e = new Eph(simulationData.ephBonus);
					e->time = simulationData.ephTime;
					n->eph = e;
					n->behaviorTimer = 0;
				} else{
					n->eph = NULL;
					n->behavior = SEARCHING;
				}
			}else {
				n->eph = NULL;
				n->behavior = SEARCHING;
			}
			
			n->behaviorTimer = simulationData.ephTime;

		}else{

		} //end of neighborhood inheritance else

		//eph reuse section. ephs can be used by types A and B interchangeably
		if(eph != NULL){
			vector<int> currentKeys;
			for(int i = 0; i < neighborsList.size(); i++){
				GsopNode neighbor = (*nodes)[neighborsList[i]];
				if(neighbor.type=='A'){
					if(simulationData.isAReuser){
						currentKeys.push_back(neighborsList[i]);
					}
				}else if(neighbor.type=='B'){
					if(simulationData.isBReuser){
						currentKeys.push_back(neighborsList[i]);
					}
				}

			}

			shuffle(currentKeys.begin(), currentKeys.end(), *eng);
			bool pegou = false;
			for(int i = 0; i < currentKeys.size(); i++){
				int ckey = currentKeys[i];
				if((*nodes)[ckey].behavior == SEARCHING){
					(*nodes)[ckey].eph = eph;
					(*nodes)[ckey].behavior = USING_SHARED;
					(*nodes)[ckey].behaviorTimer = 0;
					pegou = true;
					break;
				}
			}

			if(!pegou){
				currentKeys.clear();

				//currentKeys = keys;
				for(int i = 0; i < keys.size(); i++){
					int ckey = keys[i];
					if((*nodes)[ckey].type == 'A'){
						if(simulationData.isAReuser){
							currentKeys.push_back(keys[i]);
						}
					}else if((*nodes)[ckey].type == 'B'){
						if(simulationData.isBReuser){
							currentKeys.push_back(keys[i]);
						}
					}
				}

				if(currentKeys.size() > 0) shuffle(currentKeys.begin(), currentKeys.end(), *eng);

				for(int i = 0; i < currentKeys.size(); i++){
					int ckey = currentKeys[i];
					if((*nodes)[ckey].behavior == SEARCHING){
						(*nodes)[ckey].eph = eph;
						(*nodes)[ckey].behavior = USING_SHARED;
						(*nodes)[ckey].behaviorTimer = 0;
						pegou = true;
						break;
					}
				}
			}

			if(!pegou) delete eph;
		}
		//end of eph reuse section
	}


	//eph times update
	for(int i = 0; i < keys.size(); i++){
		int keyi = keys[i];
		GsopNode *n = &(*nodes)[keyi];
		if(n->eph != NULL){
			if(n->eph->time == 1){
				delete n->eph;
				n->eph = NULL;
				n->behavior = SEARCHING;
				n->behaviorTimer = simulationData.ephTime;
			}else{
				n->eph->time -= 1;
			}
		}
	}

	int csearchingA = 0;
	int cproducingA = 0;
	int cusingA = 0;
	int cusingsharedA = 0;
	int csearchingB = 0;
	int cproducingB = 0;
	int cusingB = 0;
	int cusingsharedB = 0;
	int ceph = 0;
	int cephshared = 0;
	//behavior update
	for(int i = 0; i < keys.size(); i++){
		int keyi = keys[i];
		GsopNode *n = &(*nodes)[keyi];

		if(simulationData.behaviorHistory){
			switch(n->behavior){
				case SEARCHING:
					if(n->type=='A')
						csearchingA++;
					else csearchingB++;
					break;
				case PRODUCING:
					if(n->type=='A')
						cproducingA++;
					else cproducingB++;
					break;
				case USING:
					if(n->type=='A')
						cusingA++;
					else cusingB++;
					if(n->eph != NULL) ceph++;
					break;
				case USING_SHARED:
					if(n->type=='A')
						cusingsharedA++;
					else cusingsharedB++;
					if(n->eph != NULL) cephshared++;
					break;
			}
		}

		if(n->behaviorTimer == 1){
			//cout<<"time "<<n->behavior<<endl;
			if(n->behavior == SEARCHING){
				if(n->type=='A'){
					if(simulationData.isAProducer){
						n->behavior = PRODUCING;
						n->behaviorTimer = simulationData.ephTime;
					}
				}else if(n->type=='B'){
					if(simulationData.isBProducer){
						n->behavior = PRODUCING;
						n->behaviorTimer = simulationData.ephTime;
					}
				}

			}else if(n->behavior == PRODUCING){
				//cout<<"gerou\n";
				n->behavior = USING;
				Eph *e = new Eph(simulationData.ephBonus);
				e->time = simulationData.ephTime;
				n->eph = e;
				n->behaviorTimer = 0;
			}
		}else if(n->behaviorTimer > 1){
			n->behaviorTimer -= 1;
		}
	}

	if(simulationData.behaviorHistory){
		*behaviorSnapshot = to_string(csearchingA)+';'+to_string(csearchingB)+';'+to_string(cproducingA)+';'+
		to_string(cproducingB)+';'+to_string(cusingA)+';'+to_string(cusingB)+';'+to_string(cusingsharedA)+';'+
		to_string(cusingsharedB)+";\n";

	}
}

void SimulationCycles::cycleV7(unordered_map<int,GsopNode> *nodes, SimulationData simulationData, mt19937 *eng){

	//Count deaths
	int dieCount = (int) ((double) nodes->size() * (simulationData.deathRate-1) );
	if(dieCount==0) dieCount++;
	vector<int> selectedKeys;
	vector<int> selectedKeysDeath;
	vector<int> keys;
	keys.resize(nodes->size());
	for(int i = 0; i < nodes->size(); i++){
		keys[i] = i;
	}
	shuffle(keys.begin(), keys.end(), *eng);

	//deaths
	for(int i = 0; i < dieCount; i++){
		selectedKeys.push_back(keys[i]);
	}

	//deaths in case of neighborhood inheritance
	if (simulationData.neighborhoodInheritance) {
		for (int i = dieCount; i < dieCount*2; i++) {
			selectedKeysDeath.push_back(keys[i]);
		}
	}

	//births
	for(int keyi = 0; keyi < selectedKeys.size(); keyi++){
		int key = selectedKeys[keyi];
		GsopNode *n = &(*nodes)[key];

		vector<int> roulette;
		vector<int> neighborsList = n->neighbors;

		for(int i = 0; i < neighborsList.size(); i++){
			GsopNode neighbor = (*nodes)[neighborsList[i]];

			int qtd = (int) (neighbor.getCoeff()*100);
			for(int j = 0; j < qtd; j++){
				roulette.push_back(neighbor.id);
			}
		}

		Eph *eph = NULL;

		GsopNode *sorteado;
		if(roulette.size()==0){
			sorteado = &(*nodes)[key];
		}else{
			uniform_int_distribution<> distr(0, roulette.size()-1);
			int chosen = distr(*eng);
			sorteado = &(*nodes)[roulette[chosen]];
		}

		sorteado->fitness++;

		if(!simulationData.neighborhoodInheritance){
			eph = (n->eph);
			n->eph = NULL;
			n->coeff = sorteado->coeff;
			n->type = sorteado->type;
			n->fitness = 0;
			if(simulationData.bEph ||  n->type=='A'){

				uniform_int_distribution<> distr(0, 99);
				int sorteioGeracao = distr(*eng);

				if (sorteioGeracao < (simulationData.ephBirthGenerationChance * 100)) {
					Eph *e = new Eph(simulationData.ephBonus);
					e->time = simulationData.ephTime;
					n->eph = e;
				}
			}
		}else{

			GsopNode *dyingNode = &(*nodes)[selectedKeysDeath[0]];

			int selectedId = dyingNode->id;
			eph = dyingNode->eph;

			//remove dying node from neighbors' neighborhoods
			for(int i = 0; i < dyingNode->neighbors.size(); i++){
				GsopNode *neighbor = &(*nodes)[dyingNode->neighbors[i]];

				vector<int> updatedNList;

				if(neighbor->id!=dyingNode->id){

					vector<int>::iterator it = find(neighbor->neighbors.begin(), neighbor->neighbors.end(), selectedId);
					neighbor->neighbors.erase(it);
				}
			}

			selectedKeysDeath.erase(selectedKeysDeath.begin());
			//generating new node
			GsopNode newNode;
			newNode.id = selectedId;
			newNode.coeff = sorteado->coeff;
			newNode.type = sorteado->type;
			newNode.fitness = 0;
			//new node neighborhood list
			vector<int> newNeighbors = sorteado->neighbors;

			nodes->erase(selectedId);

			newNode.neighbors = newNeighbors;

			if(simulationData.bEph || newNode.type == 'A'){

				uniform_int_distribution<> distr(0, 99);
				int sorteioGeracao = distr(*eng);

				if (sorteioGeracao < (simulationData.ephBirthGenerationChance * 100)) {
					Eph *e = new Eph(simulationData.ephBonus);
					e->time = simulationData.ephTime;
					newNode.eph = e;
				}
			}

			for(int i = 0 ;i < newNode.neighbors.size(); i++){
				unordered_map<int,GsopNode>::iterator it = (*nodes).find(newNode.neighbors[i]);
				if(it != (*nodes).end()){

					it->second.neighbors.push_back(selectedId);
				}
			}

			nodes->insert(pair<int,GsopNode>(selectedId,newNode));

		}

		vector<int> currentKeys;
		for(int i = 0; i < neighborsList.size(); i++){
			currentKeys.push_back(neighborsList[i]);
		}

		shuffle(currentKeys.begin(), currentKeys.end(), *eng);
		bool pegou = false;
		for(int i = 0; i < currentKeys.size(); i++){
			int ckey = currentKeys[i];
			if((*nodes)[ckey].eph ==NULL && (*nodes)[ckey].type =='A'){
				(*nodes)[ckey].eph = eph;
				pegou = true;
				break;
			}
		}

		if(!pegou){
			currentKeys.clear();

			currentKeys = keys;
			shuffle(currentKeys.begin(), currentKeys.end(), *eng);
			for(int i = 0; i < currentKeys.size(); i++){
				int ckey = currentKeys[i];
				if((*nodes)[ckey].eph ==NULL && (*nodes)[ckey].type =='A'){
					(*nodes)[ckey].eph = eph;
					pegou = true;
					break;
				}
			}
		}

		if(!pegou) delete eph;

	}

	for(int i = 0; i < keys.size(); i++){
		int keyi = keys[i];
		GsopNode *n = &(*nodes)[keyi];
		if(n->eph != NULL){
			if(n->eph->time == 1){
				delete n->eph;
				n->eph = NULL;
			}else{
				n->eph->time -= 1;
			}
		}
	}


}
