#pragma once

#define Disp7S_Tx_pin       PA2
#define Disp7S_CMD_clear    0x76
#define Disp7S_CMD_cursor   0x79
#define Disp7S_CMD_1st      0x7B
#define Disp7S_CMD_2nd      0x7C
#define Disp7S_CMD_3rd      0x7D
#define Disp7S_CMD_4th      0x7E

void initDisp7S(void);
void disp7SPrint(char*);
void disp7SPrint(float);
void disp7SPrint(float,bool);
void disp7SPrintBoil(void);
void disp7SPrintOff(void);
void disp7SSetDots(int);
