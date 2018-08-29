#include "Arduino.h"
#include "Hysteresis.h"

uint16_t currentOutput = 0;

uint16_t hystOut(float ref,float state){
    if (state<ref-HYSTERESIS_BAND){
        currentOutput = 100;
        return currentOutput;
    }
    else if (state>ref){
        currentOutput = 0;
        return currentOutput;
    }
    else{
        return currentOutput;
    }
}
