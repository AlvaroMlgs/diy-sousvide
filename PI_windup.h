#pragma once

#define PI_windup_kp 8
#define PI_windup_ki 0.4
#define PI_windup_kw 0.2   // Windup feedback constant

extern int main_cycle_period;   // Definded in arduino-controller.ino (microseconds)

uint16_t windupOut(float,float);
float windup_integrate(float,float,float);
