#ifndef MODEL_EPH
#define MODEL_EPH

class Eph{
public:
    double bonus;
    int time;
    char type;
    Eph(double b);
    Eph(double b, char t);
};

#endif