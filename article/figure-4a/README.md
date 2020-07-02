# Parameters for each simulation set

## Figure 4a

Common parameters for all simulation sets. If not set, it uses the default value. If *varies*, see next table for reference.

| parameter         | value   | default | description | 
|-|-|-|-|
| samples           | 5000|1        |number of samples to be run with current graph  | 
| cycles            | 5000|1000     |limit of simulation cycles. simulation may end without fixation of either type  | 
| ephBonus          | *varies* |0.04     |extended phenotype (eph) bonus for node type A. will be added to the baseline relative fitness of 1.0  | 
| ephBonusB         | 0.02 |-1       |eph bonus for node type B. if given -1, will have the same value of ephBonus  | 
| threads           | 72|1        |thread limit for multithreaded execution.   | 
| ephStartRatio     | 0.6 |0.6      |sets how many nodes will start at PRODUCING, USING or USING_SHARED states. 0.6 stands for 60% of all nodes.   | 
| ephBuildingRatio  | 0.6666 |0.3333   |sets how many nodes begin at PRODUCING state. 0.3333 means 33,33% of the amount calculated at ephStartRatio will be PRODUCING. Given 0.3333 and ephStartRatio of 0.6, 0.3333*0.6 	&asymp; 0.2 or aprox. 20% of all individuals. | 
| ephReusingRatio   | 0.1666|0.3333   |sets how many nodes begin at USING_SHARED state. 0.3333 means 33,33% of the amount calculated at ephStartRatio will be USING_SHARED. Given 0.3333 and ephStartRatio of 0.6, 0.3333*0.6 	&asymp; 0.2 or aprox. 20% of all individuals. ephStartRatio * (1 - ephBuildingRatio - ephReusingRatio) implies how many individuals will be in USING state. | 
| ephPopHistory     | false |false    |will print detailed sample information cycle by cycle. semicolon-separated output formatted as: cycle;type A node count;type B node count;total eph count;average node degree  | 
| behaviorHistory   |  |false    |will print detailed count of nodes separated by behavior cycle by cycle. semicolon-separated output formatted as: bhistory;sample id;cycle;A Seaching;B Searching;A building;B building;A using;B using;A using shared;B using shared;    | 
| ephTime           | 30 |30       |how many cycles an eph will last before it expires  | 
| ni                | false |false    |(*disabled*) Neighborood inheritance for new nodes. Always false at the most recent simulation ( *simulationV8() at simulation.cpp* ) | 
| sampleId          | *defined according to current graph*|0        |numerical sample identifier outputted at the same line of the result, for identification only. No effect on the simulation itself.  | 
| bEph              |  |true     |(*disabled*) defines if type B individuals can use extended phenotypes. No effect when running most recent simulation ( *simulationV8() at simulation.cpp* )  | 
| behaviorTime      |  |30       |how many cycles each node will spend at each behavior state  | 
| rBMA              | *varies* |0.3      |reuse bonus multiplier for type A individuals. 0.3 means that given ephBonus of 0.1, when a type A node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.3 = 0.03. If set to -1.0 type A individuals will not enter USING_SHARED state  | 
| rBMB              | 1 |0.9      |reuse bonus multiplier for type B individuals. 0.9 means that given ephBonusB of 0.1, when a type B node is at USING_SHARED state, it will be given a relative fitness bonus of 0.1 * 0.9 = 0.09. If set to -1.0 type B individuals will not enter USING_SHARED state  | 
| bBA               | 0.97 |0.99     |building bonus for type A individuals. given bBA 0.99, when a type A node is at PRODUCING state, it will be given a relative fitness value of 0.99. If set to -1.0 type A individuals will not enter PRODUCING state  | 
| bBB               | 0.97 |0.93     |building bonus for type B individuals. given bBB 0.97, when a type B node is at PRODUCING state, it will be given a relative fitness value of 0.93. If set to -1.0 type B individuals will not enter PRODUCING state  |
| ephBirthGenChance | |0.0      |Chance of generating an extended phenotype at birth. | 
| printPartials     | true |false    |will print partial results at each 500 cycles. semicolon-separated output formatted as: partial;cycle;type A node count;type B node cout;total eph count;sample id   | 


Values for individual simulations

ephBonus | rBMA |  folder 
-|-|-
0.02 | 1 | b2g2
0.02 | 1.5 | b2g3
0.02 | 2 | b2g4
0.02 | 2.5 | b2g5
0.02 | 3 | b2g6
0.02 | 3.5 | b2g7
0.02 | 4 | b2g8
0.03 | 0.66667 | b3g2
0.04 | 0.5 | b4g2
0.05 | 0.4 | b5g2
0.06 | 0.3333 | b6g2
0.07 | 0.28571 | b7g2
0.08 | 0.25 | b8g2
