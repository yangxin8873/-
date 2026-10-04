
#ifndef __UI_TASK_H
#define __UI_TASK_H

#include "freertos.h"

#include "ui.h"

/* 创建 UI 相关任务 */
void UI_TasksCreate(void);

/* 从其他任务发送传感器数据 */
BaseType_t UI_PostSensorData(const UI_SensorData_t *data);

/* 从其他任务发送时间 */
BaseType_t UI_PostClockData(const UI_TimeData_t *time);

/* 请求切换界面 */
BaseType_t UI_PostSwitch(void);

/* 按键扫描任务 */
void Key_Task(void *pvParameters);

/* LVGL 任务 */
void LVGL_Task(void *pvParameters);

#endif

