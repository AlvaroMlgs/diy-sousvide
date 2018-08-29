#include "LM35.h"
#include "Relay.h"
#include "Potentiometer.h"
#include "Display7S.h"
#include "Button.h"
#include "Hysteresis.h"

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
    printVal("M",CONTROL_MODE);

    float temp = getTemp();
    printVal("t",millis());
    printVal("T",temp*100);

    float potValue = potRead();
    printVal("r",potValue*100);

    updateDisp7S(potValue,temp);

    uint16_t dutyCycle = 0;
    switch(CONTROL_MODE){
        case HYSTERESIS:
            dutyCycle = hystOut(potValue,temp);
        case PI_WEIGHT:
            (void)0;
        case PI_WINDUP:
            (void)0;
        case PI_FILTER:
            (void)0;
        case EVENTS:
            (void)0;
    }
    setDutyCycle(dutyCycle);
    printVal("o",dutyCycle,1);

}

void updateDisp7S(float pot,float temp){
    // Select what to print on the 7-Segment display based on
    // how much and when the potentiometer value changed
    bool pot_display_timeout = main_current_period/1000-pot_value_time > 1000;
    if ( abs(pot_value_last-pot)>1 ){
        pot_value_last = pot;
        pot_value_time = main_current_period/1000;
        disp7SPrint(pot,true);
    }
    else{
        if (pot_display_timeout)
            disp7SPrint(temp);
        else
            disp7SPrint(pot,true);
    }
}

void printVal(char* mag, int32_t val, int8_t endline){
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
