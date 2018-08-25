#include "Arduino.h"
#include "LM35.h"

void initTemp(void){
    pinMode(sense_pin,INPUT);
    analogReadResolution(ADC_res);
}

float getTemp(void){
    int sense_raw = analogRead(sense_pin);
    //Serial.print("Raw: "); Serial.print(sense_raw); Serial.print("    ");
    float sense_volts = (float)sense_raw/(pow(2,ADC_res)-1) * ADC_maxvolts;
    //Serial.print("Volts: "); Serial.print(sense_volts); Serial.print(" V    ");
    float sense_temp = 100*sense_volts;   // 10 mV/degC
    //Serial.print("Temp: "); Serial.print(sense_temp); Serial.print(" C    ");
    //Serial.println();
    return sense_temp;
}
