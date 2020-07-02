# Instructions

## Before running the simulations

### Subfolders

At every figure-* folder, the subfolders correspond to the data points at the X axis. Build the simulation executable and copy it into each one of those subfolders.

Every subfolder will contain a gsop executable, the main.py graph generator script and the exp.sh orchestrator script.

### The exp.sh script

These simulations were originally run on a SLURM cluster, but if it is not the case, the #SBATCH directives can be safely ignored. To run the simulations on a SLURM cluster, please adjust the #SBATCH as appropriate according to your environment.

The "module load ..." commands refer to the [Environment Modules](http://modules.sourceforge.net/) software, and it is not required to run the simulation. It may be adjusted if you have different modules installed, or ignored (commented out) if your system already has the required dependencies.

#### Parameters and number of samples

The for loop inside each exp.sh script defines how many different graphs will be generated. The number of samples for each graph is defined at the "samples" parameter at the simulation executable call (at the $FILE variable). All simulations here have 5000 samples for each one of the 1000 graphs generated (totalling 5,000,000 individual samples)

### Dependencies

GNU g++ version 7.XX onwards is recommended for compiling and executing the main simulation program. 

The main.py graph generator utility requires the networkx package version 1.9 or greater.

The plot generator scripts require python packages seaborn version 0.10.0 or greater, numpy version 1.18 or greater and pandas version 1.0.2 or greater.

## Running the simulations

Call the exp.sh executable. 

The job-percent.txt file will be automatically created with the percentage of samples completed (value between 0.00 and 1.00). The calculation takes into consideration the number of loop passes into the exp.sh call. If the number of loops executed is changed, the "echo "scale=2; $i/1000" | bc > job-percent.txt" command must be changed to perform the calculation correctly.

The job-over.txt file will contain the number "1" when the simulation is over. 

The "threads" parameter passed into the simulation executable must be adjusted to suit the hardware. Ideally this number should be roughly equal to the number of virtual threads available on the system in order to maximize CPU usage and decrease processing times.

Every file containing a simulation set (5,000,000 samples) has around 1.1GB size when all processing is done, considering the parameters and number of samples in these sets. Setting the printPartials parameter to 0 may decrease this size to 200MB or less, but it is required to generate some of the figures in the article. The output file name is configured to the same name of the subfolder.

## Plotting results

Every figure-* folder has plot-*.py files. Call those files with python3 to generate the pictures. Some of them will require up to 13GB RAM during this process.

## Parameters for individual simulations

A README file containing parameters for every individual simulation at the subfolders of each figure-* folder can be found in the figure-* folders.
