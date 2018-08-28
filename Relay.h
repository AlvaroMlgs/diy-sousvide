#pragma once

#define TIME_BASE_RELAY 0x0064   // 100 prescaled steps

void initRelay(void);
void setDutyCycle(uint16_t);
void modifyBits(volatile uint32_t*,uint32_t,uint32_t);
