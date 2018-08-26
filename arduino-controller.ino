#include "Potentiometer.h"
#include "LM35.h"
#include "Display7S.h"

int relay_pin=PB4;
uint32_t main_cycle_period=1*pow(10,5);   // microseconds
uint32_t main_last_period;
uint32_t main_current_period;

void setup(void) {
    Serial.begin(115200);   // Comms with host PC
    pinMode(relay_pin,OUTPUT);      // Turns relay on and off
    digitalWrite(relay_pin,LOW);    // Start with relay off
    initTemp();     // Initialises temperature pin and sets resolution
    initDisp();     // Initialises interface to communicate with display
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
    printVal(mag,val,1);
}
