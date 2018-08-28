#pragma once

#define Pot_pin PB1
#define ADC_res      12     // Use 12 bits for maximum resolution
#define ADC_maxvolts 3.3

float potRead(void);
float potLimit(float);
