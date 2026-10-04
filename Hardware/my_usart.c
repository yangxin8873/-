#include "my_usart.h"
#include "stm32f10x.h"

myUART_TypeDef u2 = {0};

void my_usart1_Init(void)  // PA9 TX   PA10 RX
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;   // 奇偶校验
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);
    
    
    USART_Cmd(USART1, ENABLE);
}

void my_usart_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
//int fputc(int ch, FILE *f)
//{
//    my_usart_SendByte(ch);
//    return ch;
//}

/**
 * @brief ??2???
 * 
 * @param baud ???
 */
void uart2_init(uint32_t baud)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;   // 奇偶校验
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStruct);


    NVIC_InitTypeDef NVIC_InitStructure = { 0 };
    //Usart1 NVIC ??
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//?????3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//????3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
    NVIC_Init(&NVIC_InitStructure);	//??????????VIC???

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????????
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

void uart2_sendString(uint8_t* str)
{
    while(*str != '\0')
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==0);
        USART2->DR = *str;
        str++;
    }
}

uint8_t ch;
void USART2_IRQHandler(void)   
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        ch = USART2->DR;

        if(u2.count >= 256)   u2.count = 0;

        USART1->DR = ch; // ?????1

        u2.buf[u2.count] = ch;
        u2.count++;
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
        u2.isReady = 1;
        uint8_t temp = USART2->SR;
        temp = USART2->DR;
    }
}






















