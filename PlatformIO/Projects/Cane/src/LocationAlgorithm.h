#ifndef Location_Algorithm_H
#define Location_Algorithm_H

#include "string.h"



class mainAlgo
{
private:


public:
    mainAlgo () { /*constructor*/ }
enum roomType
{
    NONE,
    ROOMA,
    ROOMB,
    ROOMC,
    ROOMD,
    ROOME
};
enum directionCane
{
    STAY,
    LEFT,
    RIGHT
};


void leftDir(int curr, int dest);
void rightDir(int curr, int dest);
void currentLoc(int inforte); // tells where the current location of the user
void destLoc(); // tells where the 

};

class roomA : public mainAlgo
{
    private:
        int loc = ROOMA;
        bool follow_red = false;


    public:
        void moveToDest(int dest);
};


class roomB : public mainAlgo
{

    private:
        int loc = ROOMB;
    public:
        void moveToDest(int dest);
};


class roomC : public mainAlgo
{
    int loc = ROOMC;
    public:

        void location();
};



class roomD : public mainAlgo
{
    int loc = ROOMD;
    public:

        void location();
};



class roomE : public mainAlgo
{
    int loc = ROOME;
    public:

        void location();
};








#endif