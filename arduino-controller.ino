#include "LM35.h"
#include "Relay.h"
#include "Potentiometer.h"
#include "Display7S.h"
#include "Button.h"

#define main_cycle_period 100000    // 10^5 us = 10 Hz
uint32_t main_last_period;
uint32_t main_current_period;
float pot_value_last = 0;       // Last pot value
uint32_t pot_value_time = 0;    // Time that pot_value_last had a *significant* change

void setup(void) {
    Serial.begin(115200);   // Comms with host PC
    initTemp();     // Initialises temperature pin and sets resolution
    initRelay();    // Initialises relay pin and sets it to LOW
    initDisp7S();     // Initialises interface to communicate with display
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

    CONTROL_MODE = readButton();

    float temp = getTemp();
    printVal("t",millis());
    printVal("T",temp*100);

    float potValue = potRead();
    printVal("p",potValue*100,1);

    /* Select what to print on the 7-Segment display based on
       how much and when the potentiometer value changed      */
    bool pot_display_timeout = main_current_period/1000-pot_value_time > 1000;
    if ( abs(pot_value_last-potValue)>1 ){
        pot_value_last = potValue;
        pot_value_time = main_current_period/1000;
        disp7SPrint(potValue,true);
    }
    else{
        if (pot_display_timeout)
            disp7SPrint(temp);
        else
            disp7SPrint(potValue,true);
    }

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
