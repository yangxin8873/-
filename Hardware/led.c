#include "led.h"



void Led_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef g;
    g.GPIO_Mode = GPIO_Mode_Out_PP;
    g.GPIO_Pin = GPIO_Pin_5;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &g);  
    GPIO_Init(GPIOE, &g);  
}
