#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "delay.h"

typedef enum
{
    KEY_0_DOWN = 1,
    KEY_1_DOWN = 2,
    KEY_NO_DOWN = 0 
}KEY_ACTION;

void Key_Init(void);
KEY_ACTION Key_Scan(void);

#endif
