#include <Arduino.h>
#include "LocationAlgorithm.h"
#include "init.h"


enum BtnState {
OK = 100,
NEXT,
PREV,
CANCEL, 
  
NONE

};



void setup() {

  initializeData* m;
  m->init(); 

}

void loop() {

  Serial.println("Serial Test: Loop Test");
  delay(1000);

 ///////////////
 //get init function for buttons functions
 /////////////////


 ///////////////////
 //pass current location and destination to algorithm
 /////////////////

}