#include "GsopNode.h"

using namespace std;

GsopNode::GsopNode(){
  this->eph = NULL;
  this->fitness = 0;
}

double GsopNode::getCoeff(){
  return this->coeff+this->getBonus();
}

double GsopNode::getCoeffV8(SimulationData sd){
  double stateCoeff = this->coeff;
  double adjustedBonus = this->getBonus(&sd);

  if(this->behavior == PRODUCING){
    if(this->type == 'A'){
      stateCoeff = sd.buildingBonusA;
    } else{
      stateCoeff = sd.buildingBonusB;
    }
  } else if (this->behavior == SEARCHING) {
    if(this->type == 'A'){
      stateCoeff = sd.searchingBonusA;
    } else{
      stateCoeff = sd.searchingBonusB;
    }
  } else if(this->behavior == USING_SHARED){
    if(this->type == 'A'){
      adjustedBonus = adjustedBonus * sd.reuseBonusMultiplierA;
    } else{
      adjustedBonus = adjustedBonus * sd.reuseBonusMultiplierB;
    }
  }

  return stateCoeff + adjustedBonus;
}

double GsopNode::getBonus(SimulationData *sd){
  if(this->eph != NULL){   
    if(sd!=NULL && this->type == 'B'){
      return sd->ephBonusB;
    }	else return this->eph->bonus;
  } else return 0;
}
