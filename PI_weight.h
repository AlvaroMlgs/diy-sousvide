#pragma once

#define PI_weight_kp 6
#define PI_weight_ki 0.05
//#define PI_weight_kd 0.5
#define PI_weight_wp 0.7
#define PI_weight_wi 1
//#define PI_weight_wd 1

extern int main_cycle_period;   // Definded in arduino-controller.ino (microseconds)

uint16_t weightOut(float,float);
float weight_integrate(float,float,float);
