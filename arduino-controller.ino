#include "LM35.h"
#include "Relay.h"
#include "Potentiometer.h"
#include "Display7S.h"
#include "Button.h"
#include "Hysteresis.h"
#include "PI_weight.h"
#include "PI_windup.h"
#include "PI_filter.h"

extern int main_cycle_period;

int main_cycle_period = 100000; // 10^5 us = 10 Hz
uint32_t main_last_period;
uint32_t main_current_period;
float pot_value_last = 0;       // Last pot value
uint32_t pot_value_time = 0;    // Time that pot_value_last had a *significant* change

void setup(void) {
    Serial.begin(115200);   // Comms with host PC
    initTemp();     // Initialises temperature pin and sets resolution
    initRelay();    // Initialises relay pin and sets it to LOW
    initDisp7S();   // Initialises interface to communicate with display
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

    float temp = getTempAvg();
    printVal("t",millis());
    printVal("T",temp*100);

    float pot_value = potRead();
    printVal("r",pot_value*100);

    updateDisp7S(pot_value,temp);

    switch(CONTROL_MODE){
        case HYSTERESIS:
            duty_cycle = hystOut(pot_value,temp);
            break;
        case PI_WEIGHT:
            duty_cycle = weightOut(pot_value,temp);
            break;
        case PI_WINDUP:
            duty_cycle = windupOut(pot_value,temp);
            break;
        case PI_FILTER:
            duty_cycle = filterOut(pot_value,temp);
            break;
        case EVENTS:
            (void)0;
            break;
    }
    setDutyCycle(dutyCycle);
    printVal("o",dutyCycle,1);

}

void updateDisp7S(float pot,float temp){
    // Select what to print on the 7-Segment display based on
    // how much and when the potentiometer value changed
    bool pot_display_timeout = main_current_period/1000-pot_value_time > 3000;
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
