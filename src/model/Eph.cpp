#include "Eph.h"

Eph::Eph(double b){
  this->bonus = b;
  this->time = 0;
  this->type = 'A';
}

Eph::Eph(double b, char t){
  this->bonus = b;
  this->time = 0;
  this->type = t;
}
