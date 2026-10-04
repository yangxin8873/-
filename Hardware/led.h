#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"

#define LED_B5(x)    GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(x))
#define LED_E5(x)    GPIO_WriteBit(GPIOE, GPIO_Pin_5, (BitAction)(x))

void Led_Init(void);

#endif

