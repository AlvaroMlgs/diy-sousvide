#pragma once

#define HYSTERESIS  0
#define PI_WEIGHT   1
#define PI_WINDUP   2
#define PI_FILTER   3
#define EVENTS      4

#define BUTTON_pin      PB3
#define HYSTERESIS_pin  PB12
#define PI_WEIGHT_pin   PB13
#define PI_WINDUP_pin   PB14
#define PI_FILTER_pin   PB15
#define EVENTS_pin      PB11

extern int8_t CONTROL_MODE;

void initButton(void);
int8_t readButton(void);
void tryCycleMode(void);
uint8_t cycleMode(void);
int mode2pin(int8_t);
