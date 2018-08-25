#include "Arduino.h"
#include "Potentiometer.h"

uint16_t potRead(void){
    uint16_t pot_raw = analogRead(Pot_pin);
    uint16_t pot_tenthousands = 10000*pot_raw/pow(2,ADC_res);
    uint16_t pot_limited = potLimit(pot_tenthousands);
    return pot_limited;
}

uint16_t potLimit(uint16_t num){
    if (num>9950)
        num=9999;
    else if (num<2000)
        num=0;
    return num;
}
