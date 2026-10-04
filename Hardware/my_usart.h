#ifndef __MY_USART_H
#define __MY_USART_H

#include "stm32f10x.h"
#include <stdio.h>


typedef struct 
{
    uint8_t  buf[256];
    uint16_t count;
    int8_t   isReady;
}myUART_TypeDef;

extern myUART_TypeDef u2; 

void my_usart1_Init(void);
void my_usart_SendByte(uint8_t Byte);

void uart2_init(uint32_t baud);
void uart2_sendString(uint8_t* str);
#endif


