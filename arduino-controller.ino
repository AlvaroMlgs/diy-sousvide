#include "LM35.h"
#include "Relay.h"
#include "Potentiometer.h"
#include "Display7S.h"
#include "Controller.h"

#define main_cycle_period 100000    // 10^5 us = 10 Hz
uint32_t main_last_period;
uint32_t main_current_period;

void setup(void) {
    Serial.begin(115200);   // Comms with host PC
    initTemp();     // Initialises temperature pin and sets resolution
    initRelay();    // Initialises relay pin and sets it to LOW
    initDisp();     // Initialises interface to communicate with display
    initButton();   // Initialises the button to change modes and LEDs
    int last_period=micros();
}

void loop(void) {

    main_current_period=micros();
    if ( main_current_period-main_last_period >= main_cycle_period ){
        main_last_period=main_current_period;
        mainCycle();
    }
}

void mainCycle(void){
    float temp = getTemp();
    uint32_t temp_int=temp*1000;
    printVal("t",millis());
    printVal("T",temp_int,1);

    uint16_t pot = potRead();
    dispPrint(pot);

    setDutyCycle(pot/100);
}

void printVal(char* mag, uint32_t val, uint8_t endline){
    Serial.print(mag);
    Serial.print(val);
    if (endline)
        Serial.println();
    else
        Serial.print(",");
}

void printVal(char* mag, uint32_t val){
    printVal(mag,val,0);
}
