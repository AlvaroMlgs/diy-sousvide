#include "LM35.h"
#include "Relay.h"
#include "Potentiometer.h"
#include "Display7S.h"
#include "Button.h"
#include "Hysteresis.h"
#include "PI_weight.h"
#include "PI_windup.h"
#include "PI_filter.h"
#include "Events.h"
#include "misc.h"

extern int main_cycle_period;

int main_cycle_period = 100000; // 10^5 us = 10 Hz
uint32_t main_last_period;
uint32_t main_current_period;
float pot_value_last = 0;       // Last pot value
uint32_t pot_value_time = 0;    // Time that pot_value_last had a *significant* change
uint16_t duty_cycle = 0;

bool request_event = false;
float events_pot_value;
float events_temp;

void setup(void) {
    Serial.begin(115200);   // Comms with host PC
    initTemp();     // Initialises temperature pin and sets resolution
    initRelay();    // Initialises relay pin and sets it to LOW
    initDisp7S();   // Initialises interface to communicate with display
    initButton();   // Initialises the button to change modes and LEDs
}

void loop(void) {

    main_current_period=micros();
    if ( main_current_period-main_last_period >= main_cycle_period ){
        main_last_period=main_current_period;
        mainCycle();
    }
    if (request_event){
        duty_cycle = eventsOut(events_pot_value,events_temp);
        setDutyCycle(duty_cycle);
        request_event = false;  // Wait until next event
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
            request_event = eventsTrigger(pot_value,temp,main_current_period);
            if (request_event){  // Copy to global variables
                events_pot_value=pot_value;
                events_temp=temp;
            }
            break;
    }
    if (CONTROL_MODE!=EVENTS){  // Will be done in the main function
        setDutyCycle(duty_cycle);
    }
    if (CONTROL_MODE==EVENTS && request_event)
        printVal("o",duty_cycle);
    else
        printVal("o",duty_cycle,1);

}

void updateDisp7S(float pot,float temp){
    // Select what to print on the 7-Segment display based on
    // how much and when the potentiometer value changed
    bool pot_display_timeout = main_current_period/1000-pot_value_time > 3000;
    if ( abs(pot_value_last-pot)>1 ){   // If it was a significant change
        pot_value_last = pot;
        pot_value_time = main_current_period/1000;  // Convert to milliseconds
        disp7SPrint(pot,true);
    }
    else{
        if (pot_display_timeout)    // If enough time has passed since showing pot value
            disp7SPrint(temp);
        else
            disp7SPrint(pot,true);
    }
}

