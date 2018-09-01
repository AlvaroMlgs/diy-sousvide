#pragma once

#define PI_filter_kp    10
#define PI_filter_ki    0.4
#define PI_filter_max   15
#define PI_filter_min   -5

extern int main_cycle_period;   // Definded in arduino-controller.ino (microseconds)

uint16_t filterOut(float,float);
float filter_integrate(float,float,float);
