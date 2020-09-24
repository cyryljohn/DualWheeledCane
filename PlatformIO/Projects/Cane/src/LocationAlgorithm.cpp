#include "LocationAlgorithm.h"
#include "Arduino.h"



void roomA::moveToDest(int dest)
{
    for(int i = loc; i <= dest; i++) // number of room need to pass to destination
    {
        //////////////////
        //check color sensor
        /////////////////


        
        ////////////
        //check obstacle
        ///////////
    }

}

void roomB::moveToDest(int dest)
{
    int direction = STAY;
    (dest == ROOMA) ? leftDir(loc,dest) : rightDir(loc,dest);
}

void mainAlgo::leftDir(int loc, int dest){ // left side lane base to illustration.

    for(int i = loc; i <= dest; i--) 
    {
        //////////////////
        //check color sensor
        /////////////////


        
        ////////////
        //check obstacle
        ///////////
    }

}

void mainAlgo::rightDir(int loc, int dest){ // right lane based to  illustration

    for(int i = loc; i <= dest; i++)
    {
        //////////////////
        //check color sensor
        /////////////////


        
        ////////////
        //check obstacle
        ///////////
    }

}


