## Instructions for building and running simulations

The simulation is composed of a C++ program, which runs the simulation itself, and a helper python script which builds the initial graph. The complete simulation requires four steps:

 1. Build the simulation executable
 2. Run python script for generating a graph
 3. Run the simulation with desired parameters 
 4. Plot/analyze the simulation output

 Building the C++ project on each machine where it will be run is recommended for performance reasons.

 ### 1. Building the simulation executable

 A C++ compiler with support for the C++11 standard and multithreading is required. GNU g++ version 7.XX onwards is recommended.

 1. Type 'make' at project root
 2. Binary named 'gsop' will be created at 'dist' folder

 ### 2. Run python script for graph generation

 *networkx package version > 1.9 required.*

 Parameters:
  * operation: Set 'newGraph' for graph generation.
  * graphtype: Choose one between:
    * ba: Barabasi-albert graph
    * simple: Regular graph
    * complete: Fully connected complete graph
  * numNodes: Number of nodes
  * numEdges: Valid for simple and ba graphs.
    * ba: Sets m number of edges to attach from a new node to existing nodes, as defined in https://networkx.github.io/documentation/networkx-1.9.1/reference/generated/networkx.generators.random_graphs.barabasi_albert_graph.html
    * simple: Number of edges at each node

Sample script call:
 
    python3 main.py --operation=newGraph --graphtype=ba --numNodes=200 --numEdges=2

###  3. Simulation parameters

The following table has all the parameters the simulation binary will accept.

| parameter         | type   | default | description | 
|-|-|-|-|
| samples           | integer|1        |number of samples to be run with current graph  | 
| cycles            | integer|1000     |limit of simulation cycles. simulation may end without fixation of either type  | 
| ephBonus          | double |0.04     |extended phenotype (eph) bonus for node type A. will be added to the baseline relative fitness of 1.0  | 
| ephBonusB         | double |-1       |eph bonus for node type B. if given -1, will have the same value of ephBonus  | 
| threads           | integer|1        |thread limit for multithreaded execution.   | 
| ephStartRatio     | double |0.6      |sets how many nodes will start at PRODUCING, USING or USING_SHARED states. 0.6 stands for 60% of all nodes.   | 
| ephBuildingRatio  | double |0.3333   |sets how many nodes begin at PRODUCING state. 0.3333 means 33,33% of the amount calculated at ephStartRatio will be PRODUCING. Given 0.3333 and ephStartRatio of 0.6, 0.3333*0.6 	&asymp; 0.2 or aprox. 20% of all individuals. | 
| ephReusingRatio   | double |0.3333   |sets how many nodes begin at USING_SHARED state. 0.3333 means 33,33% of the amount calculated at ephStartRatio will be USING_SHARED. Given 0.3333 and ephStartRatio of 0.6, 0.3333*0.6 	&asymp; 0.2 or aprox. 20% of all individuals. ephStartRatio * (1 - ephBuildingRatio - ephReusingRatio) implies how many individuals will be in USING state. | 
| ephPopHistory     | boolean|false    |will print detailed sample information cycle by cycle. semicolon-separated output formatted as: cycle;type A node count;type B node count;total eph count;average node degree  | 
| behaviorHistory   | boolean|false    |will print detailed count of nodes separated by behavior cycle by cycle. semicolon-separated output formatted as: bhistory;sample id;cycle;A Seaching;B Searching;A building;B building;A using;B using;A using shared;B using shared;    | 
| ephTime           | integer|30       |how many cycles an eph will last before it expires  | 
| ni                | boolean|false    |(*disabled*) Neighborood inheritance for new nodes. Always false at the most recent simulation ( *simulationV8() at simulation.cpp* ) | 
| sampleId          | integer|0        |numerical sample identifier outputted at the same line of the result, for identification only. No effect on the simulation itself.  | 
| bEph              | boolean|true     |(*disabled*) defines if type B individuals can use extended phenotypes. No effect when running most recent simulation ( *simulationV8() at simulation.cpp* )  | 
| behaviorTime      | integer|30       |how many cycles each node will spend at each behavior state  | 
| rBMA              | double |0.3      |reuse bonus multiplier for type A individuals. 0.3 means that given ephBonus of 0.1, when a type A node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.3 = 0.03  | 
| rBMB              | double |0.9      |reuse bonus multiplier for type B individuals. 0.9 means that given ephBonusB of 0.1, when a type B node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.9 = 0.09  | 
| bBA               | double |0.99     |building bonus for type A individuals. given bBA 0.99, when a type A node is at PRODUCING state, it will be given a relative fitness value of 0.99  | 
| bBB               | double |0.93     |building bonus for type B individuals. given bBB 0.97, when a type B node is at PRODUCING state, it will be given a relative fitness value of 0.93  | 
| printPartials     | boolean|false    |will print partial results at each 500 cycles. semicolon-separated output formatted as: partial;cycle;type A node count;type B node cout;total eph count;sample id   | 


