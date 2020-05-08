#include "graphParser.h"
#include <sstream>
#include <fstream>

using namespace std;

GsopGraph * Parser::parse(string file){
	GsopGraph *g = new GsopGraph();

	ifstream in(file);
	ifstream in2("opts_"+file);

	string line;
	string line2;

	getline(in2,line2);
	int numNodes = stoi(line2);

	g->nodes.resize(numNodes);
	
	int nindex = 0;
	while(getline(in,line)){
		if(line[0] == '#'){
			continue;
		}else{
			
			string nodenum;
			string neighbor;
			istringstream nodelist(line);
			getline(nodelist,nodenum,' ');
			GsopNode n;
			while(getline(nodelist,neighbor,' ')){
				n.neighbors.push_back(stoi(neighbor));
			}
			n.id = stoi(nodenum);
			g->nodes[nindex] = n;
			nindex++;

		}
	}

	return g;
}
