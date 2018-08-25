#pragma once

#define sense_pin    PA4
#define ADC_res      12     // Use 12 bits for maximum resolution
#define ADC_maxvolts 3.3

void initTemp(void);
float getTemp(void);
