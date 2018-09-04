#include "Arduino.h"
#include "misc.h"

void printVal(char* mag, int32_t val, int8_t endline){
    Serial.print(mag);
    Serial.print(val);
    if (endline)
        Serial.println();
    else
        Serial.print(",");
}

void printVal(char* mag, int32_t val){
    printVal(mag,val,0);
}
