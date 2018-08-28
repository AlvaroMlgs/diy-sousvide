#include "Arduino.h"
#include "OneWire.h"

void OWlow(int pin){
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
}
void OWhigh(int pin){
    pinMode(pin,OUTPUT);
    digitalWrite(pin,HIGH);
}
void OWrelease(int pin){
    pinMode(pin,INPUT);
}

uint8_t OWreadpin(int pin){
    pinMode(pin,INPUT);
    return digitalRead(pin);
}

uint8_t OWreset(int pin){
    // Return 0 if the slave presence pulse is detected, 1 otherwise
    OWlow(pin);
    delayMicroseconds(480);
    OWrelease(pin);
    delayMicroseconds(70);
    uint8_t OWstate = OWreadpin(pin);
    delayMicroseconds(410);
    return OWstate;
}

void OWwritebit(int pin,uint8_t b){
    OWlow(pin);
    //delayMicroseconds(6);
    delayMicroseconds(10);
    if (b==1)
        OWhigh(pin);
    //delayMicroseconds(54);
    delayMicroseconds(55);
    if (b==0)
        OWhigh(pin);
    //delayMicroseconds(10);
    delayMicroseconds(5);
}

uint8_t OWreadbit(int pin){
    OWlow(pin);
    //delayMicroseconds(6);
    delayMicroseconds(3);
    OWrelease(pin);
    //delayMicroseconds(9);
    delayMicroseconds(10);
    uint8_t b = OWreadpin(pin);
    //delayMicroseconds(55);
    delayMicroseconds(53);
    return b;
}

void OWwritebyte(int pin,uint8_t by){
    Serial.print("Write: ");
    Serial.println(by,HEX);
    for (uint8_t i=0; i<8; i++){
        uint8_t b = by & 0x1;
        OWwritebit(pin,b);
        by >>= 1;
    }
}

uint8_t OWreadbyte(int pin){
    Serial.print("Read: ");
    uint8_t by=0x0;
    for (uint8_t i=0; i<8; i++){
        uint8_t b = OWreadbit(pin);
        Serial.print(b,HEX);
        by |= b << i;
    }
    Serial.println();
    return by;
}

void OWwritearray(int pin,uint8_t* bytes,int length){
    for (int i=0; i<length; i++){
        OWwritebyte(pin,bytes[i]);
    }
}

void OWreadarray(int pin,uint8_t* bytes,int length){
    for (int i=0; i<length; i++){
        bytes[i]=OWreadbyte(pin);
    }
}

void OWreadROM(int pin,uint8_t* rom_return){
    OWreset(pin);
    OWwritebyte(pin,CMD_read_ROM);
    for (int i=7; i>=0; i--){
        rom_return[i]=OWreadbyte(pin);
    }
}

void OWreadscpad(int pin,uint8_t* scpad_return){
    OWreset(pin);
    OWwritebyte(pin,CMD_skip_ROM);
    OWwritebyte(pin,CMD_read_scpad);
    OWreadarray(pin,scpad_return,Scratchpad_len);
}

