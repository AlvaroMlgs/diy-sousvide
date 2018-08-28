#include "Arduino.h"
#include "Display7S.h"

HardwareSerial Serial2(USART2);

void initDisp7S(){
    Serial2.begin(9600);
    Serial2.write(0x76);    // Clear display
    Serial2.write(0x7a);    // Set display brightness
    Serial2.write(255);     // Maximum brightness
    Serial2.print(8888);
}

void disp7SPrint(float num){
    char str[5];
    float num100 = num*100;
    int num_int = (int)num100;
    sprintf (str, "%04i", num_int);
    disp7SPrint(str);
}

void disp7SPrint(char* str){
    if (!strcmp(str,"9999")){
        Serial2.write(Disp7S_CMD_cursor);   // Set cursor command
        Serial2.write(0x00);                // Cursor to first position
        Serial2.write('B');
        Serial2.write('O');
        Serial2.write('I');
        Serial2.write('L');
        disp7SSetDots(0x00);    // Clear any dots
    }
    else if (!strcmp(str,"0000")){
        Serial2.write(Disp7S_CMD_cursor);   // Set cursor command
        Serial2.write(0x00);                // Cursor to first position
        Serial2.write(' ');
        Serial2.write('O');
        Serial2.write('F');
        Serial2.write('F');
        disp7SSetDots(0x00);    // Clear any dots
    }
    else{
        Serial2.print(str);
        disp7SSetDots(0b00000010);    // Display the decimal point
    }
}

void disp7SSetDots(int dots){
    // Turn on any, none, or all of the decimals.
    //  The six lowest bits in the decimals parameter sets a decimal 
    //  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
    //  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
    Serial2.write(0x77);
    Serial2.write(dots);
}
