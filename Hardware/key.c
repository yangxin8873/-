#include  "key.h"

#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)



void Key_Init(void)
{
    /* KEY0 & KEY1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;   // 上拉输入，（默认高电平，按键按下引脚为低电平）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;   // KEY0 and KEY1
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    /* KEY_UP  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

KEY_ACTION Key_Scan(void)
{
    if(KEY0 == 0 || KEY1 == 0)
    {
        Delay_ms(10);
        if(KEY0 == 0) return KEY_0_DOWN;   // 说明KEY 0 按下
        else if(KEY1 == 0)  return KEY_0_DOWN; // 说明KEY 1 按下
        
    }
    return KEY_NO_DOWN;
}






