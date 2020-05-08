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

 