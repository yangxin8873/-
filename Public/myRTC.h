#ifndef __MY_RTC_H
#define __MY_RTC_H

#include <stdint.h>

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;

    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;

} myRTC_Time_Typedef;
extern myRTC_Time_Typedef timeStruct;

void myRTC_Init(void);
void myRTC_setNewTime(uint32_t timestamp);
void timestamp_to_timeStruct(uint32_t timestamp, myRTC_Time_Typedef *tm);


#endif
