#include "init.h"

void initializeData::init(){

    #define button1 A0
    pinMode(button1, INPUT);
    Serial.begin(9600);
    Serial.println("initializing");
    delay(2000);
}