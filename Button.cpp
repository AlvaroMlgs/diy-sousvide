#include "Arduino.h"
#include "Button.h"

int8_t CONTROL_MODE;
volatile bool request_change_mode = false;
unsigned int last_mode_change = 0;  // Milliseconds
bool last_button_state = HIGH;      // Unpressed is HIGH (pullup)


void initButton(void){
    CONTROL_MODE = -1;  // Next time button is pressed start with first mode
    pinMode(BUTTON_pin,INPUT_PULLUP);
    pinMode(HYSTERESIS_pin,OUTPUT);
    pinMode(PI_WEIGHT_pin,OUTPUT);
    pinMode(PI_WINDUP_pin,OUTPUT);
    pinMode(PI_FILTER_pin,OUTPUT);

    attachInterrupt(BUTTON_pin,tryCycleMode,CHANGE);
}

void tryCycleMode(){
    request_change_mode = true;
}

int8_t readButton(){
    int current_millis = millis();
    bool debounce_pass = (current_millis - last_mode_change > 8);
    if (request_change_mode && debounce_pass){
        if (last_button_state==HIGH)
            CONTROL_MODE = cycleMode();
        request_change_mode = false;
        last_mode_change = current_millis;
        last_button_state =  digitalRead(BUTTON_pin);
    }
    return CONTROL_MODE;
}

uint8_t cycleMode(){
    digitalWrite(mode2pin(CONTROL_MODE),LOW);   // Turn off previous LED
    CONTROL_MODE += 1;
    if (CONTROL_MODE > 4)
        CONTROL_MODE = 0;
    digitalWrite(mode2pin(CONTROL_MODE),HIGH);  // Turn on current LED
    return CONTROL_MODE;
}

int mode2pin(int8_t mode){  // There is probably a better way than hard-code it
    if (mode==HYSTERESIS) return HYSTERESIS_pin;
    if (mode==PI_WEIGHT) return PI_WEIGHT_pin;
    if (mode==PI_WINDUP) return PI_WINDUP_pin;
    if (mode==PI_FILTER) return PI_FILTER_pin;
    if (mode==EVENTS) return EVENTS_pin;
}
