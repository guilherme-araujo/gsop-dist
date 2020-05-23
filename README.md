# Graph Simulator of Populations - gsop

Clone this repository with 
    
	git clone https://github.com/guilherme-araujo/gsop-dist.git
	
The instructions below will guide you through the process of compiling the simulator program and setting up its parameters, and a plot and analysis script for visualization of its results.

## Instructions for building and running simulations

The simulation is composed of a C++ program, which runs the simulation itself, and a helper python script which builds the initial graph. Bootstraping the complete simulation requires four steps:

 1. Build the simulation executable
 2. Run python script for generating a graph
 3. Run the simulation with desired parameters 
 4. Plot/analyze the simulation output

Building the C++ project on each machine where it will be run is recommended for performance reasons. Sharing the same binary with environments with a different operatring system, processor vendor/model or chipset may cause performance drops or failure to execute. This simulation has been tested on Linux kernels 3.10.X 4.15.X and 5.6.X on AMD and Intel microprocessors.

Step 1. only needs to be done once for every machine the simulation will run

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

The following table has all the parameters the simulation binary file will accept.

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
| rBMA              | double |0.3      |reuse bonus multiplier for type A individuals. 0.3 means that given ephBonus of 0.1, when a type A node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.3 = 0.03. If set to -1.0 type A individuals will not enter USING_SHARED state  | 
| rBMB              | double |0.9      |reuse bonus multiplier for type B individuals. 0.9 means that given ephBonusB of 0.1, when a type B node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.9 = 0.09. If set to -1.0 type B individuals will not enter USING_SHARED state  | 
| bBA               | double |0.99     |building bonus for type A individuals. given bBA 0.99, when a type A node is at PRODUCING state, it will be given a relative fitness value of 0.99. If set to -1.0 type A individuals will not enter PRODUCING state  | 
| bBB               | double |0.93     |building bonus for type B individuals. given bBB 0.97, when a type B node is at PRODUCING state, it will be given a relative fitness value of 0.93. If set to -1.0 type B individuals will not enter PRODUCING state  |
| ephBirthGenChance | double |0.0      |Chance of generating an extended phenotype at birth. | 
| printPartials     | boolean|false    |will print partial results at each 500 cycles. semicolon-separated output formatted as: partial;cycle;type A node count;type B node cout;total eph count;sample id   | 

Sample simulation call:

    ./gsop samples 100 ephStartRatio 0.60 ephBuildingRatio 0.667 ephReusingRatio 0.166 ephBonus 0.01 ephBonusB 0.08 threads 4 cycles 5000 ephTime 30 rBMA 8 rBMB 0.125 bBA 0.95 bBB 0.95

This command will run the simulation with the following parameters:

* 40% of all nodes will start SEARCHING, &asymp;40% of all nodes will start PRODUCING, &asymp;10% of all nodes will start USING and &asymp;10% of all nodes will start USING_SHARED
* A will be given 0.01 bonus when USING, 0.08 when USING_SHARED and -0.05 (0.95 - 1) when PRODUCING
* B will be given 0.08 bonus when USING, 0.01 when USING_SHARED and -0.05 (0.95 - 1) when PRODUCING
* All 100 simulations will run until a limit of 5000 cycles is reached
* 4 simulations are run in parallel, in different threads. The complete set of 100 samples is expected to finish 4 times faster than if the 'threads' parameter were '1' on a four-core system.
* Extended phenotypes will last for 30 cycles after being built

The expected output format is: 

    node type A count;node type B count;cycle of fixation;elapsed seconds;sample id; count of ephs attached to type A nodes;count of ephs attached to type B nodes; 

Shortened output example for the simulation with the parameters above, given a Barabasi-albert graph of 200 nodes:

    0;200;2174;1.0609;80;0;20
    39;161;-1;2.06202;78;1;14
    200;0;1989;0.756209;81;25;0
    74;126;-1;2.0669;79;12;10
    0;200;2725;1.13064;82;0;22
    200;0;1903;0.833711;83;15;0
    200;0;2622;0.895532;85;21;0
    ....

### 4. Plot/analyze the simulation output

Proposed plotting scripts and examples are avaliable at the plot/ folder of this repository.

The following scripts require the python packages *pandas, numpy and seaborn*

* plot/convert.py refactors output data to a more plot-friendly format. It assumes the printPartials parameter was set to 1 (true) in the simulation and the limit is at 5000 cycles
* plot/plot.py takes the file outputted by convert.py as a parameter and outputs three files. The example assumes a graph of 500 nodes
    * A boxplot with a distribution of nodes for each type every 500 cycles up to 5000.
    * fixc-{file name}.txt is a comma-separated file containing
        * first line: samples with fixation of type A;with fixation of type B;undefined simulations (none fixed)
        * second line: % of type A fixed samples;% of type B fixed samples;% of undefined samples;% of type A nodes on undefined samples;% of type B nodes on undefined samples
    * perc-{file name}.txt outputs the numerical summary ( python function *pandas.DataFrame.describe* ) of type A and type B nodes, in that order.
