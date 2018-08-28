#include "Arduino.h"
#include "Display7S.h"

HardwareSerial Serial2(USART2);
uint8_t dots_byte;

void initDisp7S(){
    Serial2.begin(9600);
    Serial2.write(0x76);    // Clear display
    Serial2.write(0x7a);    // Set display brightness
    Serial2.write(255);     // Maximum brightness
    Serial2.print(8888);
}

void disp7SPrint(float num, bool colon){
    disp7SPrint(num);
    bool has_decimal = (dots_byte>>1)&1;
    if (has_decimal && colon){
        dots_byte |= 1<<4;          // Add colon
        disp7SSetDots(dots_byte);   // Update dots
    }
}

void disp7SPrint(float num){
    dots_byte = 0x00;   // Start with no dots
    float num100 = num*100;
    int num_int = (int)num100;
    if (num_int == 9999){
        disp7SPrintBoil();
        dots_byte &= ~(1<<4);   // Remove colon
    }
    else if (num_int == 0){
        disp7SPrintOff();
        dots_byte &= ~(1<<4);   // Remove colon
    }
    else{
        char str[5];
        sprintf (str, "%04i", num_int);
        Serial2.print(str);
        dots_byte |= 1<<1;      // Add decimal point
    }
    disp7SSetDots(dots_byte);
}

void disp7SPrintBoil(){
    Serial2.write(Disp7S_CMD_cursor);   // Set cursor command
    Serial2.write(0x00);                // Cursor to first position
    Serial2.write('B');
    Serial2.write('O');
    Serial2.write('I');
    Serial2.write('L');
    disp7SSetDots(0x00);    // Clear any dots
}
void disp7SPrintOff(){
    Serial2.write(Disp7S_CMD_cursor);   // Set cursor command
    Serial2.write(0x00);                // Cursor to first position
    Serial2.write(' ');
    Serial2.write('O');
    Serial2.write('F');
    Serial2.write('F');
    disp7SSetDots(0x00);    // Clear any dots
}

void disp7SSetDots(int dots){
    // Turn on any, none, or all of the decimals.
    //  The six lowest bits in the decimals parameter sets a decimal 
    //  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
    //  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
    Serial2.write(0x77);
    Serial2.write(dots);
}
