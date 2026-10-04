#include "myRTC.h"
#include "stm32f10x.h"

myRTC_Time_Typedef timeStruct = {0};
/**
 * @brief rtc初始化
 * 
 */
void myRTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//开启PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);		//开启BKP的时钟

    /* 访问备份寄存器前需要先使能 */
    PWR_BackupAccessCmd(ENABLE);  // 

    // 访问备份寄存器，查看RTC是否为第一次初始化
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)	// 成立说明RTC没有被初始化
    {
        //开启LSE时钟
        RCC_LSEConfig(RCC_LSE_ON);  
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	//等待LSE稳定

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				//选择RTCCLK来源为LSE
		RCC_RTCCLKCmd(ENABLE);								//RTCCLK使能

        /* 等待RTC寄存器同步 */
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();		//等待上一次操作完成

        /* 设置RTC预分频器，预分频后的计数频率为1Hz */
        RTC_SetPrescaler(32768 - 1);	
		RTC_WaitForLastTask();	   //等待上一次操作完成

        /* 设置初始时间 */
        // RTC_SetCounter(time_cnt);						//将秒计数器写入到RTC的CNT中
	    // RTC_WaitForLastTask();							//等待上一次操作完成

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);  // 写入备份寄存器，代表RTC已初始化
    }
    else
    {
        /*  RTC已经初始化
         * 只需要重新同步
         */
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();	//等待上一次操作完成
    }
}
/**
 * @brief 设置rtc的cnt计数器
 * 
 * @param timestamp 新的时间戳
 */
void myRTC_setNewTime(uint32_t timestamp)
{
    RTC_SetCounter(timestamp);
    RTC_WaitForLastTask();	//等待上一次操作完成
}

/* 判断闰年 */
static uint8_t is_leap_year(uint16_t year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

/**
 * @brief  将 Unix 时间戳（秒）转换为日期时间结构体
 * @param  timestamp: 从 1970-01-01 00:00:00 UTC 开始的秒数
 * @param  tm: 输出结构体指针
 * @retval 无
 */
void timestamp_to_timeStruct(uint32_t timestamp, myRTC_Time_Typedef *tm)
{
    // if (tm == NULL) {
    //     return;
    // }

    uint32_t days;
    uint32_t secs;
    timestamp += 8 * 3600;
    /* 1. 分离出天数与当天秒数 */
    days = timestamp / 86400U;          /* 86400 = 24 * 60 * 60 */
    secs = timestamp % 86400U;

    /* 2. 计算时、分、秒 */
    tm->hour   = (uint8_t)(secs / 3600U);
    tm->minute = (uint8_t)((secs % 3600U) / 60U);
    tm->second = (uint8_t)(secs % 60U);

    /* 3. 计算年 */
    uint16_t year = 1970;
    while (1) {
        uint16_t days_in_year = is_leap_year(year) ? 366 : 365;
        if (days < days_in_year) {
            break;
        }
        days -= days_in_year;
        year++;
    }
    tm->year = year;

    /* 4. 计算月、日 */
    const uint8_t month_days[12] = {31, 28, 31, 30, 31, 30,
                                    31, 31, 30, 31, 30, 31};
    uint8_t month = 0;
    while (1) {
        uint8_t dim = month_days[month];
        if (month == 1 && is_leap_year(year)) { /* 2 月闰年 29 天 */
            dim = 29;
        }
        if (days < dim) {
            break;
        }
        days -= dim;
        month++;
    }
    tm->month = month + 1;
    tm->day   = (uint8_t)(days + 1);
}


