#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "my_usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lvgl.h"
#include "ui_task.h"
#include "lv_port_disp.h"
#include "myRTC.h"
#include "key.h"
#include "esp32.h"

// 队列句柄
QueueHandle_t printQueueHandle = NULL;

void task1(void* param)
{
    for(;;)
    {
        GPIOB->ODR ^= GPIO_Pin_5;
        vTaskDelay(500);
        xQueueSend(printQueueHandle, "task1", 0);
    }
}

void myRTC_task(void* param)
{
    
    uint32_t timestamp = 0;


    TickType_t last_wake = xTaskGetTickCount();
    for(;;)
    {
        timestamp = RTC_GetCounter();
        timestamp_to_timeStruct(timestamp, &timeStruct);
        // printf("20%02d-%02d-%02d %02d:%02d:%02d\r\n",
        //                         timeStruct.year % 100,
        //                         timeStruct.month,
        //                         timeStruct.day,
        //                         timeStruct.hour,
        //                         timeStruct.minute,
        //                         timeStruct.second);
        
        UI_PostClockData((UI_TimeData_t *)&timeStruct);

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(1000));
    }
}

void printTask(void* param)
{
    char buf[20] = { 0 };
    for(;;)
    {
        xQueueReceive(printQueueHandle, buf, portMAX_DELAY);
        taskENTER_CRITICAL();
        // printf("%s\r\n", buf);
        taskEXIT_CRITICAL();
    }
}


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	 // (4位全部用于抢占)
    my_usart1_Init();
    myRTC_Init();
    uart2_init(115200);
    Led_Init();
    Key_Init();
    printf("111\r\n");
    Delay_ms(1000);
    printf("2222\r\n");
    lv_init();
    lv_port_disp_init();
    
    /* 创建 UI 任务和队列 */
    UI_TasksCreate();
    
    // 创建队列
    printQueueHandle = xQueueCreate(5, 20); // 队列最大有5个成员，每个成员的大小为20字节 
    xTaskCreate(task1, "task1", 100, NULL, 1, NULL);
    
    
    atCmdSend("AT+RST\r\n", "OK", 3000);
    uart2_sendString("AT\r\n");
    Delay_ms(500);
    uart2_sendString("AT\r\n");
    Delay_ms(500);
    printf("开始连接WiFi\r\n");
    
    u32 timestamp = 0;
        // 联网校准
    wifi_Connect("esp", "esp12345678");
    timestamp = getNewTime();
    myRTC_setNewTime(timestamp);
    
    
    xTaskCreate(myRTC_task, "myRTC_task", 128, NULL, 5, NULL);
    
    xTaskCreate(printTask, "printTask", 128, NULL, 1, NULL);
    
    
    
    
    isUseOs = 1;
    vTaskStartScheduler();
    
    while(1);
    
}


void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}



