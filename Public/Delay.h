#ifndef __DELAY_H
#define __DELAY_H

#include "stdint.h"

extern uint8_t isUseOs;

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif
