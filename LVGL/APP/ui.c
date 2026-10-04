
#include "ui.h"
#include <stdio.h>

/* 屏幕对象 */
static lv_obj_t *screen_clock;
static lv_obj_t *screen_sensor;

/* 时钟界面控件 */
static lv_obj_t *label_time;
static lv_obj_t *label_date;

/* 环境监测界面控件 */
static lv_obj_t *label_temp;
static lv_obj_t *label_humi;
static lv_obj_t *label_co2;

/* 当前界面 */
static UI_Screen_t current_screen = UI_SCREEN_CLOCK;

/* 屏幕尺寸：根据实际 LCD 修改 */
#define LCD_WIDTH       320
#define LCD_HEIGHT      240

/* 时钟界面初始化 */
static void UI_CreateClockScreen(void)
{
    /* 创建 Screen */
    screen_clock = lv_obj_create(NULL, NULL);

    /* 设置背景色 */
    lv_obj_set_style_local_bg_color(
        screen_clock,
        LV_OBJ_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_MAKE(23, 25, 31)
    );

    lv_obj_set_style_local_bg_opa(
        screen_clock,
        LV_OBJ_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_OPA_COVER
    );

    /* 创建时间 Label */
    label_time = lv_label_create(screen_clock, NULL);

    lv_label_set_text(label_time, "00:00");

    lv_obj_set_style_local_text_color(
        label_time,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_WHITE
    );

    lv_obj_set_style_local_text_font(
        label_time,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        &lv_font_montserrat_40
    );

    lv_obj_align(
        label_time,
        NULL,
        LV_ALIGN_CENTER,
        0,
        -20
    );

    /* 创建日期 Label */
    label_date = lv_label_create(screen_clock, NULL);

    lv_label_set_text(label_date, "2026-10-02");

    lv_obj_set_style_local_text_color(
        label_date,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_MAKE(220, 220, 225)
    );

    lv_obj_set_style_local_text_font(
        label_date,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        &lv_font_montserrat_20
    );

    lv_obj_align(
        label_date,
        label_time,
        LV_ALIGN_OUT_BOTTOM_MID,
        0,
        10
    );
}

// /* 环境监测界面初始化 */
// static void UI_CreateSensorScreen(void)
// {
//     screen_sensor = lv_obj_create(NULL, NULL);

//     /* 白色背景 */
//     lv_obj_set_style_local_bg_color(
//         screen_sensor,
//         LV_OBJ_PART_MAIN,
//         LV_STATE_DEFAULT,
//         LV_COLOR_WHITE
//     );

//     lv_obj_set_style_local_bg_opa(
//         screen_sensor,
//         LV_OBJ_PART_MAIN,
//         LV_STATE_DEFAULT,
//         LV_OPA_COVER
//     );

//     /* 标题 */
//     lv_obj_t *title = lv_label_create(screen_sensor, NULL);

//     lv_label_set_text(title, "Environment Monitor");

//     lv_obj_set_style_local_text_color(
//         title,
//         LV_LABEL_PART_MAIN,
//         LV_STATE_DEFAULT,
//         LV_COLOR_BLACK
//     );

//     lv_obj_set_style_local_text_font(
//         title,
//         LV_LABEL_PART_MAIN,
//         LV_STATE_DEFAULT,
//         &lv_font_montserrat_20
//     );

//     lv_obj_align(
//         title,
//         NULL,
//         LV_ALIGN_IN_TOP_MID,
//         0,
//         12
//     );

//     /* 温度 */
//     label_temp = lv_label_create(screen_sensor, NULL);
//     lv_label_set_text(label_temp, "Temperature: --.- C");

//     /* 湿度 */
//     label_humi = lv_label_create(screen_sensor, NULL);
//     lv_label_set_text(label_humi, "Humidity: -- %");

//     /* CO2 */
//     label_co2 = lv_label_create(screen_sensor, NULL);
//     lv_label_set_text(label_co2, "CO2: ---- ppm");

//     /* 统一设置字体和颜色 */
//     lv_obj_t *labels[] = {
//         label_temp,
//         label_humi,
//         label_co2
//     };

//     for (uint8_t i = 0; i < 3; i++)
//     {
//         lv_obj_set_style_local_text_color(
//             labels[i],
//             LV_LABEL_PART_MAIN,
//             LV_STATE_DEFAULT,
//             LV_COLOR_BLACK
//         );

//         lv_obj_set_style_local_text_font(
//             labels[i],
//             LV_LABEL_PART_MAIN,
//             LV_STATE_DEFAULT,
//             &lv_font_montserrat_20
//         );
//     }

//     /* 控件位置 */
//     lv_obj_align(
//         label_temp,
//         NULL,
//         LV_ALIGN_IN_TOP_LEFT,
//         20,
//         65
//     );

//     lv_obj_align(
//         label_humi,
//         label_temp,
//         LV_ALIGN_OUT_BOTTOM_LEFT,
//         0,
//         22
//     );

//     lv_obj_align(
//         label_co2,
//         label_humi,
//         LV_ALIGN_OUT_BOTTOM_LEFT,
//         0,
//         22
//     );
// }

/* 创建环境监测页面 */
static void UI_CreateSensorScreen(void)
{
    screen_sensor = lv_obj_create(NULL, NULL);

    // UI_SetBlackBackground(screen_sensor);
    // 设置背景颜色
    lv_obj_set_style_local_bg_color(
            screen_sensor,
            LV_OBJ_PART_MAIN,
            LV_STATE_DEFAULT,
            LV_COLOR_BLACK
        );

    lv_obj_set_style_local_bg_opa(
        screen_sensor,
        LV_OBJ_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_OPA_COVER
    );

    /* 温度 */
    label_temp = lv_label_create(screen_sensor, NULL);

    lv_label_set_text(label_temp, "Temperature: --.- C");

    lv_obj_set_style_local_text_color(
        label_temp,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_WHITE
    );

    lv_obj_set_style_local_text_font(
        label_temp,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        &lv_font_montserrat_20
    );

    lv_obj_align(
        label_temp,
        NULL,
        LV_ALIGN_CENTER,
        0,
        -65
    );

    /* 湿度 */
    label_humi = lv_label_create(screen_sensor, NULL);

    lv_label_set_text(label_humi, "Humidity: -- %");

    lv_obj_set_style_local_text_color(
        label_humi,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_WHITE
    );

    lv_obj_set_style_local_text_font(
        label_humi,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        &lv_font_montserrat_20
    );

    lv_obj_align(
        label_humi,
        NULL,
        LV_ALIGN_CENTER,
        0,
        0
    );

    /* CO2 */
    label_co2 = lv_label_create(screen_sensor, NULL);

    lv_label_set_text(label_co2, "CO2: ---- ppm");

    lv_obj_set_style_local_text_color(
        label_co2,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        LV_COLOR_WHITE
    );

    lv_obj_set_style_local_text_font(
        label_co2,
        LV_LABEL_PART_MAIN,
        LV_STATE_DEFAULT,
        &lv_font_montserrat_20
    );

    lv_obj_align(
        label_co2,
        NULL,
        LV_ALIGN_CENTER,
        0,
        65
    );
}

/* UI 初始化 */
void UI_Init(void)
{
    UI_CreateClockScreen();
    UI_CreateSensorScreen();

    current_screen = UI_SCREEN_CLOCK;

    lv_scr_load(screen_clock);
}

/* 切换界面 */
void UI_SwitchScreen(void)
{
    if (current_screen == UI_SCREEN_CLOCK)
    {
        current_screen = UI_SCREEN_SENSOR;
        lv_scr_load(screen_sensor);
    }
    else
    {
        current_screen = UI_SCREEN_CLOCK;
        lv_scr_load(screen_clock);
    }
}

/* 更新温湿度和 CO2 */
void UI_UpdateSensor(const UI_SensorData_t *data)
{
    if (data == NULL)
        return;

    lv_label_set_text_fmt(
        label_temp,
        "Temperature: %.1f C",
        (double)data->temperature
    );

    lv_label_set_text_fmt(
        label_humi,
        "Humidity: %.1f %%",
        (double)data->humidity
    );

    lv_label_set_text_fmt(
        label_co2,
        "CO2: %u ppm",
        (unsigned int)data->co2
    );
}

/* 更新时间 */
void UI_UpdateClock(const UI_TimeData_t *time)
{
    char buf[24];

    if (time == NULL)
        return;

    snprintf(
        buf,
        sizeof(buf),
        "%02u:%02u:%02u",
        (unsigned int)time->hour,
        (unsigned int)time->minute,
        (unsigned int)time->second
    );

    lv_label_set_text(label_time, buf);

    snprintf(
        buf,
        sizeof(buf),
        "%04u-%02u-%02u",
        (unsigned int)time->year,
        (unsigned int)time->month,
        (unsigned int)time->day
    );

    lv_label_set_text(label_date, buf);
}

/* 获取当前界面 */
UI_Screen_t UI_GetCurrentScreen(void)
{
    return current_screen;
}





