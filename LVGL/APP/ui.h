
#ifndef __UI_H
#define __UI_H

#include "lvgl.h"
#include <stdint.h>

/* 界面类型 */
typedef enum
{
    UI_SCREEN_CLOCK = 0,
    UI_SCREEN_SENSOR
} UI_Screen_t;

/* 传感器数据 */
typedef struct
{
    float temperature;
    float humidity;
    uint16_t co2;
} UI_SensorData_t;

/* 日期和时间 */
typedef struct
{   
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    
} UI_TimeData_t;



/* 初始化界面，只允许在 LVGL_Task 中调用 */
void UI_Init(void);

/* 切换界面，只允许在 LVGL_Task 中调用 */
void UI_SwitchScreen(void);

/* 更新传感器显示，只允许在 LVGL_Task 中调用 */
void UI_UpdateSensor(const UI_SensorData_t *data);

/* 更新时间显示，只允许在 LVGL_Task 中调用 */
void UI_UpdateClock(const UI_TimeData_t *time);

/* 获取当前界面 */
UI_Screen_t UI_GetCurrentScreen(void);

#endif
