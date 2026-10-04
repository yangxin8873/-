
#include "ui_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f10x.h"

/* 按键 GPIO 配置：请根据开发板修改 */
#define KEY1_GPIO           GPIOE
#define KEY1_PIN            GPIO_Pin_4

/* 按键低电平按下 */
#define KEY1_PRESSED        Bit_RESET

/* 按键扫描周期 */
#define KEY_SCAN_MS         10

/* 消抖时间 */
#define KEY_DEBOUNCE_COUNT  3

/* 任务配置 */
#define LVGL_TASK_STACK     512
#define LVGL_TASK_PRIORITY  3

#define KEY_TASK_STACK      256
#define KEY_TASK_PRIORITY   2

/* UI 消息类型 */
typedef enum
{
    UI_MSG_SWITCH = 0,
    UI_MSG_SENSOR,
    UI_MSG_CLOCK
} UI_MsgType_t;

/* UI 消息结构体 */
typedef struct
{
    UI_MsgType_t type;

    union
    {
        UI_SensorData_t sensor;
        UI_TimeData_t clock;
    } data;

} UI_Msg_t;

/* UI 消息队列 */
static QueueHandle_t UI_Queue = NULL;

/* 读取按键状态 */
static uint8_t KEY1_Read(void)
{
    if (GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN)
        == KEY1_PRESSED)
    {
        return 1;
    }

    return 0;
}

/* 向 UI 队列发送消息 */
static BaseType_t UI_SendMessage(const UI_Msg_t *msg)
{
    if (UI_Queue == NULL || msg == NULL)
    {
        return pdFAIL;
    }

    return xQueueSend(UI_Queue, msg, 0);
}

/* 请求切换界面 */
BaseType_t UI_PostSwitch(void)
{
    UI_Msg_t msg = {0};

    msg.type = UI_MSG_SWITCH;

    return UI_SendMessage(&msg);
}

/* 发送传感器数据 */
BaseType_t UI_PostSensorData(const UI_SensorData_t *data)
{
    UI_Msg_t msg = {0};

    if (data == NULL)
        return pdFAIL;

    msg.type = UI_MSG_SENSOR;
    msg.data.sensor = *data;

    return UI_SendMessage(&msg);
}

/* 发送时间数据 */
BaseType_t UI_PostClockData(const UI_TimeData_t *time)
{
    UI_Msg_t msg = {0};

    if (time == NULL)
        return pdFAIL;

    msg.type = UI_MSG_CLOCK;
    msg.data.clock = *time;

    return UI_SendMessage(&msg);
}

/* 按键任务 */
void Key_Task(void *pvParameters)
{
    uint8_t raw_state;
    uint8_t stable_state = 0;
    uint8_t debounce_count = 0;

    (void)pvParameters;

    while (1)
    {
        raw_state = KEY1_Read();

        if (raw_state != stable_state)
        {
            debounce_count++;

            if (debounce_count >= KEY_DEBOUNCE_COUNT)
            {
                stable_state = raw_state;
                debounce_count = 0;

                /* 只在稳定按下的瞬间发送事件 */
                if (stable_state == 1)
                {
                    UI_PostSwitch();
                }
            }
        }
        else
        {
            debounce_count = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(KEY_SCAN_MS));
    }
}

/* LVGL 消息处理 */
static void UI_ProcessMessage(const UI_Msg_t *msg)
{
    switch (msg->type)
    {
        case UI_MSG_SWITCH:
        {
            UI_SwitchScreen();
            break;
        }

        case UI_MSG_SENSOR:
        {
            UI_UpdateSensor(&msg->data.sensor);
            break;
        }

        case UI_MSG_CLOCK:
        {
            UI_UpdateClock(&msg->data.clock);
            break;
        }

        default:
            break;
    }
}

/* LVGL 任务 */
void LVGL_Task(void *pvParameters)
{
    UI_Msg_t msg;

    (void)pvParameters;

    /* 初始化 LVGL 界面 */
    UI_Init();

    while (1)
    {
        /*
         * 等待消息最多 10ms，
         * 避免任务一直阻塞而影响 LVGL 刷新。
         */
        if (xQueueReceive(
                UI_Queue,
                &msg,
                pdMS_TO_TICKS(10)) == pdPASS)
        {
            UI_ProcessMessage(&msg);
        }

        /* LVGL 7.11 定时任务处理 */
        lv_task_handler();

        /* 让出 CPU */
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/* 创建 UI 任务 */
void UI_TasksCreate(void)
{
    /* 创建消息队列 */
    UI_Queue = xQueueCreate(8, sizeof(UI_Msg_t));

    configASSERT(UI_Queue != NULL);

    /* 创建 LVGL 任务 */
    xTaskCreate(
        LVGL_Task,
        "LVGL",
        LVGL_TASK_STACK,
        NULL,
        LVGL_TASK_PRIORITY,
        NULL
    );

    /* 创建按键任务 */
    xTaskCreate(
        Key_Task,
        "KEY",
        KEY_TASK_STACK,
        NULL,
        KEY_TASK_PRIORITY,
        NULL
    );
}

