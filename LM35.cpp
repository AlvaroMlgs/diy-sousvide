#include "Arduino.h"
#include "LM35.h"

float temp_avg = 0;

void initTemp(void){
    pinMode(sense_pin,INPUT);
    analogReadResolution(ADC_res);
}

float getTemp(void){
    int sense_raw = analogRead(sense_pin);
    float sense_volts = (float)sense_raw/(pow(2,ADC_res)-1) * ADC_maxvolts;
    float sense_temp = 100*sense_volts;   // 10 mV/degC
    return sense_temp;
}

float getTempAvg(){   // Exponential Weighted Moving Average (EWMA)
    float alpha = 0.5;
    temp_avg = alpha*getTemp() + (1-alpha)*temp_avg;
    return temp_avg;
}
