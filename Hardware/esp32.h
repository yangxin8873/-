#ifndef ESP32_H
#define ESP32_H

#include "stdint.h"
#include "delay.h"
#include "string.h"
#include "cJSON.h"
typedef struct
{
    int isWifiConnect;
    int isServerConnect;
}ESP_Typedef;
extern ESP_Typedef esp;

int8_t atCmdSend(uint8_t* cmd, const char* ack, uint16_t waitTime);
int8_t wifi_Connect(const char* ssid, const char* pwd);
int8_t wifi_Connect_Server(const char* ip, int port);
int8_t wifi_Disconnect_Server(void);
int8_t wifi_GetState(const char* ssid);
uint32_t getNewTime(void);
#endif
