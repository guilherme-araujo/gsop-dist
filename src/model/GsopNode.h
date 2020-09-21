#ifndef MODEL_GSOPNODE
#define MODEL_GSOPNODE

#include <string>
#include <vector>

#include "Eph.h"
#include "SimulationData.h"

enum NodeState{
  SEARCHING,    //behaviorTimer begins at ephTime
  PRODUCING,    //behaviorTimer begins at ephTime
  USING,        //behavior lasts remaining ephTime (behaviorTimer == 0)
  USING_SHARED  //behavior lasts remaining ephTime (behaviorTimer == 0)

};

class GsopNode{
public:

    std::string hash;
    char type;
    int id;
    int val;
    double coeff;
    int fitness;
    Eph *eph;
    int behaviorTimer;
    NodeState behavior;
    bool cheater;

    std::vector<int> neighbors;

    GsopNode();
    double getCoeff();
    double getCoeffV8(SimulationData sd);
    double getBonus(SimulationData *sd = NULL);
};

#endif
