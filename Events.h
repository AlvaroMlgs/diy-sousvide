#pragma once

#define EVENTS_kp   5
#define EVENTS_ki   0.4
#define EVENTS_elim 1       // degrees
#define EVENTS_tlim 3000    // milliseconds
#define EVENTS_imax 15  // Integration saturation
#define EVENTS_imin -3  // Integration saturation

bool eventsTrigger(float,float,uint32_t);
uint16_t eventsOut(float,float);
float eventsIntegrate(float,float,float,uint32_t);
