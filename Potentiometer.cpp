#include "Arduino.h"
#include "Potentiometer.h"

float potRead(void){
    uint16_t pot_raw = analogRead(Pot_pin);
    float pot_percent = 100*(float)pot_raw/pow(2,ADC_res);
    float pot_limited = potLimit(pot_percent);
    return pot_limited;
}

float potLimit(float num){
    if (num>99.50)
        num=99.99;
    else if (num<20.00)
        num=0;
    return num;
}
