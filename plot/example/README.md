This example was run with the following commands:

Graph generation:

    python3 main.py --operation=newGraph --graphtype=ba --numNodes=500 --numEdges=2

Simulation:

    ./gsop samples 100 ephStartRatio 0.60 ephBuildingRatio 0.667 ephReusingRatio 0.166 ephBonus 0.01 ephBonusB 0.08 threads 4 cycles 5000 ephTime 30 rBMA 8 rBMB 0.125 bBA 0.95 bBB 0.95 printPartials 1 > exp.csv

Conversion and plotting:

    python3 convert.py exp.csv
    python plot.py conv_exp.csv
