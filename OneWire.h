#pragma once

#define Scratchpad_len      9

#define CMD_search_ROM      0xF0
#define CMD_read_ROM        0x33
#define CMD_match_ROM       0x55
#define CMD_skip_ROM        0xCC
#define CMD_convert_T       0x44
#define CMD_write_scpad     0x4E
#define CMD_read_scpad      0xBE
#define CMD_save_EEPROM     0x48
#define CMD_recall_EEPROM   0xB8
#define CMD_read_power      0xB4

void OWlow(int);
void OWhigh(int);
void OWrelease(int);
uint8_t OWreadpin(int);
uint8_t OWreset(int);
void OWwritebit(int,uint8_t);
uint8_t OWreadbit(int);
void OWwritebyte(int,uint8_t);
uint8_t OWreadbyte(int);
void OWwritearray(int,uint8_t*,int);
void OWreadROM(int,uint8_t*);
void OWreadscpad(int,uint8_t*);

