#pragma once

#define HYSTERESIS  0
#define PI_WEIGHT   1
#define PI_WINDUP   2
#define PI_FILTER   3
#define EVENTS      4

#define BUTTON_pin      PA1
#define HYSTERESIS_pin  PA5
#define PI_WEIGHT_pin   PA6
#define PI_WINDUP_pin   PA7
#define PI_FILTER_pin   PB0
#define EVENTS_pin      PB1

extern int8_t CONTROL_MODE;

void initButton(void);
int8_t readButton(void);
void tryCycleMode(void);
uint8_t cycleMode(void);
int mode2pin(int8_t);
